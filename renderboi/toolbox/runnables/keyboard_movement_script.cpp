#include "keyboard_movement_script.hpp"

#include <exception>

#include <renderboi/core/frame_of_reference.hpp>

#include <renderboi/window/enums.hpp>

namespace Renderboi
{

using Ref = FrameOfReference;

KeyboardMovementScript::KeyboardMovementScript(
    const BasisProviderPtr basisProvider,
    const float speed,
    const float sprintMultiplier
) :
    _basisProvider(basisProvider),
    _moveSpeed(speed),
    _sprintMultiplier(sprintMultiplier),
    _movementFlags{ false },
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
    if (_movementFlags[_IndexForward])
        position += _basisProvider->forward() * velocity;
    if (_movementFlags[_IndexBackward])
        position -= _basisProvider->forward() * velocity;
    if (_movementFlags[_IndexLeft])
        position += _basisProvider->left() * velocity;
    if (_movementFlags[_IndexRight])
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
            _movementFlags[_IndexForward] = true;
            break;
        case KeyboardMovementAction::Backward:
            _movementFlags[_IndexBackward] = true;
            break;
        case KeyboardMovementAction::Left:
            _movementFlags[_IndexLeft] = true;
            break;
        case KeyboardMovementAction::Right:
            _movementFlags[_IndexRight] = true;
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
            _movementFlags[_IndexForward] = false;
            break;
        case KeyboardMovementAction::Backward:
            _movementFlags[_IndexBackward] = false;
            break;
        case KeyboardMovementAction::Left:
            _movementFlags[_IndexLeft] = false;
            break;
        case KeyboardMovementAction::Right:
            _movementFlags[_IndexRight] = false;
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
    if (_movementFlags[_IndexForward] && _movementFlags[_IndexBackward])
    {
        _movementFlags[_IndexForward] = false;
        _movementFlags[_IndexBackward] = false;
    }
    if (_movementFlags[_IndexLeft] && _movementFlags[_IndexRight])
    {
        _movementFlags[_IndexLeft] = false;
        _movementFlags[_IndexRight] = false;
    }
}

std::string to_string(const KeyboardMovementAction& action)
{
    static bool runOnce = false;
    static std::unordered_map<KeyboardMovementAction, std::string> enumNames;

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

}//namespace Renderboi
