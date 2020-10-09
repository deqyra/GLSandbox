#include "fps_camera_script.hpp"

#include "../scene/components/camera_component.hpp"

#include "../frame_of_reference.hpp"
using Ref = FrameOfReference;

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

void FPSCameraScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    // Enable or disable directional movement flags depending on key input
    // W (forward)
    if (key == Key::W)
    {
        // If pressed or on repeat, move forward
        if (action == Action::Press || action == Action::Repeat)
            _movement[IndexForward] = true;
        // If released, stop moving forward
        if (action == Action::Release)
            _movement[IndexForward] = false;
    }
    // S (backward)
    if (key == Key::S)
    {
        if (action == Action::Press || action == Action::Repeat)
            _movement[IndexBackward] = true;
        if (action == Action::Release)
            _movement[IndexBackward] = false;
    }
    // A (left)
    if (key == Key::A)
    {
        if (action == Action::Press || action == Action::Repeat)
            _movement[IndexLeft] = true;
        if (action == Action::Release)
            _movement[IndexLeft] = false;
    }
    // D (right)
    if (key == Key::D)
    {
        if (action == Action::Press || action == Action::Repeat)
            _movement[IndexRight] = true;
        if (action == Action::Release)
            _movement[IndexRight] = false;
    }
    // For any key event, update sprint mode depending on Shift key status
    if (mods & Mod::Shift)
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

void FPSCameraScript::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
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
    glm::vec3 position = sceneObject->transform.getPosition();

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
    sceneObject->transform.setPosition<Ref::Parent>(position);
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