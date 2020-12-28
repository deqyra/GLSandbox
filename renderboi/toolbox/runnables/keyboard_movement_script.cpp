#include "keyboard_movement_script.hpp"

#include <exception>

#include <renderboi/core/frame_of_reference.hpp>
using Ref = FrameOfReference;

#include <renderboi/window/enums.hpp>

#include <cpptools/enum_map.hpp>

KeyboardMovementScript::KeyboardMovementScript(
    const BasisProviderPtr basisProvider,
    const float speed,
    const float sprintMultiplier
) :
    _basisProvider(basisProvider),
    _moveSpeed(speed),
    _sprintMultiplier(sprintMultiplier),
    _movement{ false },
    _sprint(false)
{
    if (!basisProvider) throw std::runtime_error("KeyboardMovementScript: cannot construct from a null FrontProviderPtr.");
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
    if (_movement[_IndexForward])
        position += _basisProvider->forward() * velocity;
    if (_movement[_IndexBackward])
        position -= _basisProvider->forward() * velocity;
    if (_movement[_IndexLeft])
        position += _basisProvider->left() * velocity;
    if (_movement[_IndexRight])
        position -= _basisProvider->left() * velocity;

    // Update parent position
    _sceneObject->transform.setPosition<Ref::Parent>(position);
}

void KeyboardMovementScript::setSceneObject(const SceneObjectPtr sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "KeyboardMovementScript: null scene object pointer was provided.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
}

KeyboardMovementScript* KeyboardMovementScript::clone() const
{
    return new KeyboardMovementScript(_basisProvider, _moveSpeed, _sprintMultiplier);
}

void KeyboardMovementScript::triggerAction(const GLWindowPtr window, const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movement[_IndexForward] = true;
            break;
        case KeyboardMovementAction::Backward:
            _movement[_IndexBackward] = true;
            break;
        case KeyboardMovementAction::Left:
            _movement[_IndexLeft] = true;
            break;
        case KeyboardMovementAction::Right:
            _movement[_IndexRight] = true;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = true;
            break;
    }

    cancelOppositeDirections();
}

void KeyboardMovementScript::stopAction(const GLWindowPtr window, const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movement[_IndexForward] = false;
            break;
        case KeyboardMovementAction::Backward:
            _movement[_IndexBackward] = false;
            break;
        case KeyboardMovementAction::Left:
            _movement[_IndexLeft] = false;
            break;
        case KeyboardMovementAction::Right:
            _movement[_IndexRight] = false;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = false;
            break;
    }

    cancelOppositeDirections();
}

ControlSchemeManagerPtr<KeyboardMovementAction> KeyboardMovementScript::getDefaultControlScheme() const
{
    ControlSchemeManagerPtr<KeyboardMovementAction>
    schemeManager = std::make_shared<ControlSchemeManager<KeyboardMovementAction>>();

    using Window::Input::Key;
    schemeManager->bindControl(Control(Key::W), KeyboardMovementAction::Forward);
    schemeManager->bindControl(Control(Key::A), KeyboardMovementAction::Left);
    schemeManager->bindControl(Control(Key::S), KeyboardMovementAction::Backward);
    schemeManager->bindControl(Control(Key::D), KeyboardMovementAction::Right);
    schemeManager->bindControl(Control(Key::LeftShift), KeyboardMovementAction::Sprint);

    return schemeManager;
}

void KeyboardMovementScript::cancelOppositeDirections()
{
    // Cancel directions if opposite
    if (_movement[_IndexForward] && _movement[_IndexBackward])
    {
        _movement[_IndexForward] = false;
        _movement[_IndexBackward] = false;
    }
    if (_movement[_IndexLeft] && _movement[_IndexRight])
    {
        _movement[_IndexLeft] = false;
        _movement[_IndexRight] = false;
    }
}

namespace std
{
    string to_string(const KeyboardMovementAction& action)
    {
        static bool runOnce = false;
        static unordered_enum_map<KeyboardMovementAction, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[KeyboardMovementAction::Forward]   = "Forward";
            enumNames[KeyboardMovementAction::Backward]  = "Backward";
            enumNames[KeyboardMovementAction::Left]      = "Left";
            enumNames[KeyboardMovementAction::Right]     = "Right";
            enumNames[KeyboardMovementAction::Sprint]    = "Sprint";

            runOnce = true;
        }

        auto it = enumNames.find(action);
        if (it != enumNames.end()) return it->second;

        return "Unknown";
    }
}
