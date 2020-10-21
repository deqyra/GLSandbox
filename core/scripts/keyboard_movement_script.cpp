#include "keyboard_movement_script.hpp"

#include <exception>

#include "../frame_of_reference.hpp"
using Ref = FrameOfReference;

KeyboardMovementScript::KeyboardMovementScript(BasisProviderPtr basisProvider, float speed, float sprintMultiplier) :
    _basisProvider(basisProvider),
    _moveSpeed(speed),
    _sprintMultiplier(sprintMultiplier),
    _movement{ false },
    _sprint(false)
{
    if (!basisProvider) throw std::runtime_error("KeyboardMovementScript: cannot construct from a null FrontProviderPtr.");
}

void KeyboardMovementScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
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

void KeyboardMovementScript::update(float timeElapsed)
{
    // Compute distance to cover in this frame
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= _sprintMultiplier;

    // Retrieve the linked scene object.
    glm::vec3 position = _sceneObject->transform.getPosition();

    // Depending on which directional flags were raised, compute new position
    if (_movement[IndexForward])
        position += _basisProvider->forward() * velocity;
    if (_movement[IndexBackward])
        position -= _basisProvider->forward() * velocity;
    if (_movement[IndexLeft])
        position += _basisProvider->left() * velocity;
    if (_movement[IndexRight])
        position -= _basisProvider->left() * velocity;

    // Update parent position
    _sceneObject->transform.setPosition<Ref::Parent>(position);
}

void KeyboardMovementScript::setSceneObject(SceneObjectPtr sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "KeyboardMovementScript (script ID " + std::to_string(Script::id) + ", input processor ID " + std::to_string(InputProcessor::id) + ") was a null scene object pointer.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
}

KeyboardMovementScript* KeyboardMovementScript::clone()
{
    return new KeyboardMovementScript(_basisProvider, _moveSpeed, _sprintMultiplier);
}