#include "keyboard_movement_script.hpp"

#include <exception>

#include <renderboi/core/frame_of_reference.hpp>
using Ref = FrameOfReference;

#include <cpptools/enum_map.hpp>

KeyboardMovementScript::KeyboardMovementScript(BasisProviderPtr basisProvider, float speed, float sprintMultiplier) :
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
        std::string s = "KeyboardMovementScript: null scene object pointer was provided.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
}

KeyboardMovementScript* KeyboardMovementScript::clone()
{
    return new KeyboardMovementScript(_basisProvider, _moveSpeed, _sprintMultiplier);
}

void KeyboardMovementScript::triggerAction(GLWindowPtr window, const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movement[IndexForward] = true;
            break;
        case KeyboardMovementAction::Backward:
            _movement[IndexBackward] = true;
            break;
        case KeyboardMovementAction::Left:
            _movement[IndexLeft] = true;
            break;
        case KeyboardMovementAction::Right:
            _movement[IndexRight] = true;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = true;
            break;
    }

    cancelOppositeDirections();
}

void KeyboardMovementScript::stopAction(GLWindowPtr window, const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movement[IndexForward] = false;
            break;
        case KeyboardMovementAction::Backward:
            _movement[IndexBackward] = false;
            break;
        case KeyboardMovementAction::Left:
            _movement[IndexLeft] = false;
            break;
        case KeyboardMovementAction::Right:
            _movement[IndexRight] = false;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = false;
            break;
    }

    cancelOppositeDirections();
}

void KeyboardMovementScript::cancelOppositeDirections()
{
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
