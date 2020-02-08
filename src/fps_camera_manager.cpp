/**
    GLTest, fps_camera_manager.hpp
    Purpose: Implementation of class FPSCameraManager. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 06/02/2020
*/
#include "fps_camera_manager.hpp"

FPSCameraManager::FPSCameraManager(glm::vec3 position, float yaw, float pitch, glm::vec3 up) :
    _camera(position, yaw, pitch, up),
    _moveSpeed(SPEED),
    _mouseSensitivity(SENSITIVITY),
    _movement{false},
    _sprint(false),
    _lastMouseX(0),
    _lastMouseY(0),
    _mouseWasUpdatedOnce(false)
{

}

glm::mat4 FPSCameraManager::getViewMatrix()
{
    return _camera.getViewMatrix();
}

void FPSCameraManager::processKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void FPSCameraManager::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
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

    _camera.processRotation(yawOffset, pitchOffset);
    _lastMouseX = (float) xpos;
    _lastMouseY = (float) ypos;
}

void FPSCameraManager::processMouseScroll(float scrollOffset)
{

}

void FPSCameraManager::updateCamera(float timeDelta)
{
    float velocity = timeDelta * _moveSpeed;
    if (_sprint)
        velocity *= SPRINT_MUTLIPLIER;

    if (_movement[DIR_INDEX_FORWARD])
        _camera.processMovement(Direction::FORWARD, velocity);
    if (_movement[DIR_INDEX_BACKWARD])
        _camera.processMovement(Direction::BACKWARD, velocity);
    if (_movement[DIR_INDEX_LEFT])
        _camera.processMovement(Direction::LEFT, velocity);
    if (_movement[DIR_INDEX_RIGHT])
        _camera.processMovement(Direction::RIGHT, velocity);
}

glm::vec3 FPSCameraManager::getPosition()
{
    return _camera.getPosition();
}