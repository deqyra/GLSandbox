#include "fps_camera_script.hpp"

#include "../include/GLFW/glfw3.h"
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
    // Enable or disable directional movement flags depending on key input
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
    // If the mouse was never updated before, record its position and skip this update
    if (!_mouseWasUpdatedOnce)
    {
        _lastMouseX = (float) xpos;
        _lastMouseY = (float) ypos;
        _mouseWasUpdatedOnce = true;
        return;
    }
    // This is to avoid a huge mouse jump upon entering the window with the mouse

    // Compute offsets
    float yawOffset = (float)(xpos - _lastMouseX) * _mouseSensitivity;
    // Y offset reversed since y-coordinates range from bottom to top
    float pitchOffset = (float)(_lastMouseY - ypos) * _mouseSensitivity;

    // Translate to Euler angles
    _camera->processRotation(yawOffset, pitchOffset);
    // Record mouse position
    _lastMouseX = (float) xpos;
    _lastMouseY = (float) ypos;
}

void FPSCameraScript::update(float timeElapsed)
{
    // Compute distance to cover in this frame
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= SPRINT_MUTLIPLIER;

    // Retrieve the linked scene object (i.e the camera)
    SceneObjectPtr sceneObject = _sceneObject.lock();
    glm::vec3 position = sceneObject->getPosition();

    // Depending on which directional flags were raised, compute new position
    if (_movement[DIR_INDEX_FORWARD])
        position += _camera->front() * velocity;
    if (_movement[DIR_INDEX_BACKWARD])
        position -= _camera->front() * velocity;
    if (_movement[DIR_INDEX_LEFT])
        position -= _camera->right() * velocity;
    if (_movement[DIR_INDEX_RIGHT])
        position += _camera->right() * velocity;

    // Update camera position
    sceneObject->setPosition(position);
}

void FPSCameraScript::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;

    // Retrieve camera component
    SceneObjectPtr realSceneObject = sceneObject.lock();
    std::shared_ptr<CameraComponent> cameraComponent = realSceneObject->getComponent<CameraComponent>();

    if (!cameraComponent)
    {
        std::string s = "FPSCameraScript (script ID " + std::to_string(Script::id) + ", input processor ID " + std::to_string(InputProcessor::id) + ") was attached to a scene object with no camera component.";
        throw std::runtime_error(s.c_str());
    }

    _camera = cameraComponent->getCamera();
}
