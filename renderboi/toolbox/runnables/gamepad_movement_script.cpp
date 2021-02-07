#include "gamepad_movement_script.hpp"

#include <stdexcept>

#include <renderboi/core/frame_of_reference.hpp>

#include <renderboi/window/enums.hpp>

namespace Renderboi
{

using Ref = FrameOfReference;

GamepadMovementScript::GamepadMovementScript(
    const BasisProviderPtr basisProvider,
    const float speed,
    const float sprintMultiplier
) :
    _basisProvider(basisProvider),
    _direction{0.f, 0.f},
    _moveSpeed(speed),
    _sprintMultiplier(sprintMultiplier),
    _sprint(false)
{
    if (!basisProvider) throw std::runtime_error("GamepadMovementScript: cannot construct from a null BasisProvider pointer.");
}

void GamepadMovementScript::update(float timeElapsed)
{
    // Compute distance to cover in this frame
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= _sprintMultiplier;

    // Retrieve the linked scene object.
    glm::vec3 position = _sceneObject->transform.getPosition();

    glm::vec2 movement = velocity * _direction;
    position += _basisProvider->left() * movement.x;
    position += _basisProvider->forward() * movement.y;

    // Update parent position
    _sceneObject->transform.setPosition<Ref::Parent>(position);
}

void GamepadMovementScript::setSceneObject(const SceneObjectPtr sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "GamepadMovementScript: null scene object pointer was provided.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
}

GamepadMovementScript* GamepadMovementScript::clone() const
{
    return new GamepadMovementScript(_basisProvider, _moveSpeed, _sprintMultiplier);
}

void GamepadMovementScript::processButton(const GamepadPtr gamepad, const Window::Input::Gamepad::Button button, const Window::Input::Action action)
{
    if (button == Window::Input::Gamepad::Button::A)
        _sprint = (action == Window::Input::Action::Press);
}

void GamepadMovementScript::processAxis(const GamepadPtr gamepad, const Window::Input::Gamepad::Axis axis, const float value)
{
    if (axis == Window::Input::Gamepad::Axis::LeftX)
        _direction.x = -value;

    if (axis == Window::Input::Gamepad::Axis::LeftY)
        _direction.y = -value;
}

}//namespace Renderboi
