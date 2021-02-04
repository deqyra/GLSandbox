#include "../enums.hpp"
#include "gamepad.hpp"
#include "gamepad_input_processor.hpp"
#include "gamepad_manager.hpp"
#include "gamepad_state.hpp"

#include <unordered_map>
#include <utility>

namespace Renderboi
{

using Axis = Window::Input::Gamepad::Axis;
using Button = Window::Input::Gamepad::Button;

Gamepad::Gamepad(const GamepadManagerPtr manager, const Window::Input::Joystick slot) :
    _state(),
    _enabled(false),
    _axisDeadZones(DefaultAxisDeadZones()),
    _axisWasInDeadZone(),
    _inputProcessor(std::make_shared<GamepadInputProcessor>()),
    _manager(manager),
    slot(slot)
{
    _axisWasInDeadZone[Axis::LeftX]         = true;
    _axisWasInDeadZone[Axis::LeftY]         = true;
    _axisWasInDeadZone[Axis::RightX]        = true;
    _axisWasInDeadZone[Axis::RightY]        = true;
    _axisWasInDeadZone[Axis::LeftTrigger]   = true;
    _axisWasInDeadZone[Axis::RightTrigger]  = true;
}

void Gamepad::enable()
{
    if (!_enabled)
    {
        _manager->startGamepadPolling(slot);

        _enabled = true;
    }
}

void Gamepad::disable()
{
    if (_enabled)
    {
        _manager->stopGamepadPolling(slot);

        _enabled = false;
    }
}

void Gamepad::setEnabled(const bool enabled)
{
    enabled ? enable() : disable();
}

const GamepadState& Gamepad::getState() const
{
    return _state;
}

bool Gamepad::isPressed(const Button button) const
{
    return _state[button];
}

float Gamepad::getValue(const Axis axis) const
{
    return _state[axis];
}

GamepadInputProcessorPtr Gamepad::getInputProcessor() const
{
    return _inputProcessor;
}

void Gamepad::registerInputProcessor(const GamepadInputProcessorPtr inputProcessor)
{
    _inputProcessor = inputProcessor;
    if (_inputProcessor == nullptr)
    {
        _inputProcessor = std::make_shared<GamepadInputProcessor>();
    }
}

std::pair<float, float> Gamepad::getAxisDeadZone(const Axis axis) const
{
    return _axisDeadZones.at(axis);
}

void Gamepad::setAxisDeadZone(const Axis axis, std::pair<float, float> deadZone)
{
    _axisDeadZones[axis] = deadZone;
}

const std::unordered_map<Axis, std::pair<float, float>>& Gamepad::DefaultAxisDeadZones()
{
    static std::unordered_map<Axis, std::pair<float, float>> defaultDeadZones;
    static bool runOnce = false;

    if (!runOnce)
    {
        defaultDeadZones[Axis::LeftX]        = {-.1f, .1f};
        defaultDeadZones[Axis::LeftY]        = {-.1f, .1f};
        defaultDeadZones[Axis::RightX]       = {-.1f, .1f};
        defaultDeadZones[Axis::RightY]       = {-.1f, .1f};
        defaultDeadZones[Axis::LeftTrigger]  = {-1.f, -.95f};
        defaultDeadZones[Axis::RightTrigger] = {-1.f, -.95f};
    }

    return defaultDeadZones;
}

const std::unordered_map<Axis, float>& Gamepad::AxisRestValues()
{
    static std::unordered_map<Axis, float> axisRestValues;
    static bool runOnce = false;

    if (!runOnce)
    {
        axisRestValues[Axis::LeftX]         =  0.f;
        axisRestValues[Axis::LeftY]         =  0.f;
        axisRestValues[Axis::RightX]        =  0.f;
        axisRestValues[Axis::RightY]        =  0.f;
        axisRestValues[Axis::LeftTrigger]   = -1.f;
        axisRestValues[Axis::RightTrigger]  = -1.f;
    }

    return axisRestValues;
}

void Gamepad::_processConnected()
{
    _inputProcessor->processConnected(slot);
}

void Gamepad::_processDisconnected()
{
    _inputProcessor->processDisconnected(slot);
}

void Gamepad::_setNewState(const GamepadState newState)
{
    _compareAndFireEvents(_state, newState);
    _state = newState;
}

void Gamepad::_compareAndFireEvents(const GamepadState& before, const GamepadState& after)
{
    using Window::Input::Action;

    if (before.A != after.A)
    {
        after.A
            ? _inputProcessor->processButton(slot, Button::A, Action::Press)
            : _inputProcessor->processButton(slot, Button::A, Action::Release);
    }

    if (before.B != after.B)
    {
        after.B
            ? _inputProcessor->processButton(slot, Button::B, Action::Press)
            : _inputProcessor->processButton(slot, Button::B, Action::Release);
    }

    if (before.X != after.X)
    {
        after.X
            ? _inputProcessor->processButton(slot, Button::X, Action::Press)
            : _inputProcessor->processButton(slot, Button::X, Action::Release);
    }

    if (before.Y != after.Y)
    {
        after.Y
            ? _inputProcessor->processButton(slot, Button::Y, Action::Press)
            : _inputProcessor->processButton(slot, Button::Y, Action::Release);
    }

    if (before.LeftBumper != after.LeftBumper)
    {
        after.LeftBumper
            ? _inputProcessor->processButton(slot, Button::LeftBumper, Action::Press)
            : _inputProcessor->processButton(slot, Button::LeftBumper, Action::Release);
    }

    if (before.RightBumper != after.RightBumper)
    {
        after.RightBumper
            ? _inputProcessor->processButton(slot, Button::RightBumper, Action::Press)
            : _inputProcessor->processButton(slot, Button::RightBumper, Action::Release);
    }

    if (before.DPadUp != after.DPadUp)
    {
        after.DPadUp
            ? _inputProcessor->processButton(slot, Button::DPadUp, Action::Press)
            : _inputProcessor->processButton(slot, Button::DPadUp, Action::Release);
    }

    if (before.DPadRight != after.DPadRight)
    {
        after.DPadRight
            ? _inputProcessor->processButton(slot, Button::DPadRight, Action::Press)
            : _inputProcessor->processButton(slot, Button::DPadRight, Action::Release);
    }

    if (before.DPadDown != after.DPadDown)
    {
        after.DPadDown
            ? _inputProcessor->processButton(slot, Button::DPadDown, Action::Press)
            : _inputProcessor->processButton(slot, Button::DPadDown, Action::Release);
    }

    if (before.DPadLeft != after.DPadLeft)
    {
        after.DPadLeft
            ? _inputProcessor->processButton(slot, Button::DPadLeft, Action::Press)
            : _inputProcessor->processButton(slot, Button::DPadLeft, Action::Release);
    }

    if (before.LeftThumb != after.LeftThumb)
    {
        after.LeftThumb
            ? _inputProcessor->processButton(slot, Button::LeftThumb, Action::Press)
            : _inputProcessor->processButton(slot, Button::LeftThumb, Action::Release);
    }

    if (before.RightThumb != after.RightThumb)
    {
        after.RightThumb
            ? _inputProcessor->processButton(slot, Button::RightThumb, Action::Press)
            : _inputProcessor->processButton(slot, Button::RightThumb, Action::Release);
    }

    if (before.Start != after.Start)
    {
        after.Start
            ? _inputProcessor->processButton(slot, Button::Start, Action::Press)
            : _inputProcessor->processButton(slot, Button::Start, Action::Release);
    }

    if (before.Select != after.Select)
    {
        after.Select
            ? _inputProcessor->processButton(slot, Button::Select, Action::Press)
            : _inputProcessor->processButton(slot, Button::Select, Action::Release);
    }

    if (before.Home != after.Home)
    {
        after.Home
            ? _inputProcessor->processButton(slot, Button::Home, Action::Press)
            : _inputProcessor->processButton(slot, Button::Home, Action::Release);
    }

    bool dead = true;

    dead = _valueIsInAxisDeadZone(after.LeftX, Axis::LeftX);
    if (!_axisWasInDeadZone[Axis::LeftX] && dead)
    {
        _axisWasInDeadZone[Axis::LeftX] = true;
        _inputProcessor->processAxis(slot, Axis::LeftX, AxisRestValues().at(Axis::LeftX));
    }
    else if (!dead && (before.LeftX != after.LeftX))
    {
        _inputProcessor->processAxis(slot, Axis::LeftX, after.LeftX);
    }

    dead = _valueIsInAxisDeadZone(after.LeftY, Axis::LeftY);
    if (!_axisWasInDeadZone[Axis::LeftY] && dead)
    {
        _axisWasInDeadZone[Axis::LeftY] = true;
        _inputProcessor->processAxis(slot, Axis::LeftY, AxisRestValues().at(Axis::LeftY));
    }
    else if (!dead && (before.LeftY != after.LeftY))
    {
        _inputProcessor->processAxis(slot, Axis::LeftY, after.LeftY);
    }

    dead = _valueIsInAxisDeadZone(after.RightX, Axis::RightX);
    if (!_axisWasInDeadZone[Axis::RightX] && dead)
    {
        _axisWasInDeadZone[Axis::RightX] = true;
        _inputProcessor->processAxis(slot, Axis::RightX, AxisRestValues().at(Axis::RightX));
    }
    else if (!dead && (before.RightX != after.RightX))
    {
        _inputProcessor->processAxis(slot, Axis::RightX, after.RightX);
    }

    dead = _valueIsInAxisDeadZone(after.RightY, Axis::RightY);
    if (!_axisWasInDeadZone[Axis::RightY] && dead)
    {
        _axisWasInDeadZone[Axis::RightY] = true;
        _inputProcessor->processAxis(slot, Axis::RightY, AxisRestValues().at(Axis::RightY));
    }
    else if (!dead && (before.RightY != after.RightY))
    {
        _inputProcessor->processAxis(slot, Axis::RightY, after.RightY);
    }

    dead = _valueIsInAxisDeadZone(after.LeftTrigger, Axis::LeftTrigger);
    if (!_axisWasInDeadZone[Axis::LeftTrigger] && dead)
    {
        _axisWasInDeadZone[Axis::LeftTrigger] = true;
        _inputProcessor->processAxis(slot, Axis::LeftTrigger, AxisRestValues().at(Axis::LeftTrigger));
    }
    else if (!dead && (before.LeftTrigger != after.LeftTrigger))
    {
        _inputProcessor->processAxis(slot, Axis::LeftTrigger, after.LeftTrigger);
    }

    dead = _valueIsInAxisDeadZone(after.RightTrigger, Axis::RightTrigger);
    if (!_axisWasInDeadZone[Axis::RightTrigger] && dead)
    {
        _axisWasInDeadZone[Axis::RightTrigger] = true;
        _inputProcessor->processAxis(slot, Axis::RightTrigger, AxisRestValues().at(Axis::RightTrigger));
    }
    else if (!dead && (before.RightTrigger != after.RightTrigger))
    {
        _inputProcessor->processAxis(slot, Axis::RightTrigger, after.RightTrigger);
    }
}

bool Gamepad::_valueIsInAxisDeadZone(const float& value, const Axis& axis) const
{
    return (value >= _axisDeadZones.at(axis).first) && (value <= _axisDeadZones.at(axis).second);
}

}
