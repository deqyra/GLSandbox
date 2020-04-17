#include "fps_camera_script.hpp"

#include "../include/GLFW/glfw3.h"
#include "../scene/components/camera_component.hpp"

FPSCameraScript::FPSCameraScript(float speed, float sensitivity, float sprintMultiplier) :
    _moveSpeed(speed),
    _lookSensitivity(sensitivity),
    _sprintMultiplier(sprintMultiplier),
    _movement{ false },
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
            _movement[IndexForward] = true;
        // If released, stop moving forward
        if (action == GLFW_RELEASE)
            _movement[IndexForward] = false;
    }
    // S (backward)
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[IndexBackward] = true;
        if (action == GLFW_RELEASE)
            _movement[IndexBackward] = false;
    }
    // A (left)
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[IndexLeft] = true;
        if (action == GLFW_RELEASE)
            _movement[IndexLeft] = false;
    }
    // D (right)
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            _movement[IndexRight] = true;
        if (action == GLFW_RELEASE)
            _movement[IndexRight] = false;
    }
    // For any key event, update sprint mode depending on Shift key status
    if (mods & GLFW_MOD_SHIFT)
        _sprint = true;
    else
        _sprint = false;

    // Cancel directions if opposite
    if (_movement[IndexForward] && _movement[IndexBackward])
    {
        _movement[IndexForward] = false;
        _movement[IndexBackward] = false;
    }
    if (_movement[IndexLeft] && _movement[IndexRight])
    {
        _movement[IndexLeft] = false;
        _movement[IndexRight] = false;
    }
}

void FPSCameraScript::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{
    // If the mouse was never updated before, record its position and skip this update
    if (!_mouseWasUpdatedOnce)
    {
        _lastMouseX = (float)xpos;
        _lastMouseY = (float)ypos;
        _mouseWasUpdatedOnce = true;
        return;
    }
    // This is to avoid a huge mouse jump upon entering the window with the mouse

    // Compute offsets
    float yawOffset = (float)(xpos - _lastMouseX) * _lookSensitivity;
    // Y offset reversed since y-coordinates range from bottom to top
    float pitchOffset = (float)(_lastMouseY - ypos) * _lookSensitivity;

    // Translate to Euler angles
    _camera->processRotation(yawOffset, pitchOffset);
    // Record mouse position
    _lastMouseX = (float)xpos;
    _lastMouseY = (float)ypos;
}

void FPSCameraScript::update(float timeElapsed)
{
    // Compute distance to cover in this frame
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= _sprintMultiplier;

    // Retrieve the linked scene object (i.e the camera)
    SceneObjectPtr sceneObject = _sceneObject.lock();
    glm::vec3 position = sceneObject->getPosition();

    // Depending on which directional flags were raised, compute new position
    if (_movement[IndexForward])
        position += _camera->front() * velocity;
    if (_movement[IndexBackward])
        position -= _camera->front() * velocity;
    if (_movement[IndexLeft])
        position -= _camera->right() * velocity;
    if (_movement[IndexRight])
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

FPSCameraScript* FPSCameraScript::clone()
{
    return new FPSCameraScript(_moveSpeed, _lookSensitivity, _sprintMultiplier);
}