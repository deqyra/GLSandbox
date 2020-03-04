#include "fps_camera_script.hpp"

#include "../include/GLFW/glfw3.h"
#include "../direction.hpp"
#include "../scene/components/camera_component.hpp"

FPSCameraScript::FPSCameraScript() :
    _moveSpeed(SPEED),
    _mouseSensitivity(SENSITIVITY),
    _movement{false},
    _sprint(false),
    _lastMouseX(0),
    _lastMouseY(0),
    _mouseWasUpdatedOnce(false)
{

}

void FPSCameraScript::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Four directional keys input processing
    // W (forward)
    if (key == GLFW_KEY_W)
    {
        // If pressed or on repeat, move forward
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[DIR_INDEX_FORWARD] = true;
        // If released, stop moving forward
        if (action == GLFW_RELEASE)
            _movement[DIR_INDEX_FORWARD] = false;
    }
    // S (backward)
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[DIR_INDEX_BACKWARD] = true;
        if (action == GLFW_RELEASE)
            _movement[DIR_INDEX_BACKWARD] = false;
    }
    // A (left)
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[DIR_INDEX_LEFT] = true;
        if (action == GLFW_RELEASE)
            _movement[DIR_INDEX_LEFT] = false;
    }
    // D (right)
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[DIR_INDEX_RIGHT] = true;
        if (action == GLFW_RELEASE)
            _movement[DIR_INDEX_RIGHT] = false;
    }
    // For any key event, update sprint mode depending on Shift key status
    if (mods & GLFW_MOD_SHIFT)
        _sprint = true;
    else
        _sprint = false;

    // Cancel directions if opposite
    if (_movement[DIR_INDEX_FORWARD] && _movement[DIR_INDEX_BACKWARD])
    {
        _movement[DIR_INDEX_FORWARD] = false;
        _movement[DIR_INDEX_BACKWARD] = false;
    }
    if (_movement[DIR_INDEX_LEFT] && _movement[DIR_INDEX_RIGHT])
    {
        _movement[DIR_INDEX_LEFT] = false;
        _movement[DIR_INDEX_RIGHT] = false;
    }
}

void FPSCameraScript::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{
    if (!_mouseWasUpdatedOnce)
    {
        _lastMouseX = (float) xpos;
        _lastMouseY = (float) ypos;
        _mouseWasUpdatedOnce = true;
        return;
    }

    float yawOffset = (float)(xpos - _lastMouseX) * _mouseSensitivity;
    float pitchOffset = (float)(_lastMouseY - ypos) * _mouseSensitivity; // Y offset reversed since y-coordinates range from bottom to top

    _camera->processRotation(yawOffset, pitchOffset);
    _lastMouseX = (float) xpos;
    _lastMouseY = (float) ypos;
}

void FPSCameraScript::update(float timeElapsed)
{
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= SPRINT_MUTLIPLIER;

    SceneObjectPtr sceneObject = _sceneObject.lock();
    glm::vec3 position = sceneObject->getPosition();

    if (_movement[DIR_INDEX_FORWARD])
        position += _camera->front() * velocity;
    if (_movement[DIR_INDEX_BACKWARD])
        position -= _camera->front() * velocity;
    if (_movement[DIR_INDEX_LEFT])
        position -= _camera->right() * velocity;
    if (_movement[DIR_INDEX_RIGHT])
        position += _camera->right() * velocity;

    sceneObject->setPosition(position);
}

void FPSCameraScript::setSceneObject(SceneObjectWPtr sceneObject)
{
    InputProcessingScript::setSceneObject(sceneObject);
    SceneObjectPtr realSceneObject = sceneObject.lock();
    auto cameraComponent = realSceneObject->getComponent<CameraComponent>();
    auto realComponent = cameraComponent.lock();
    _camera = realComponent->camera;
}
