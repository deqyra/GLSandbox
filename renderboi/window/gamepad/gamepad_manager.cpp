#include "../enums.hpp"
#include "abstract_gamepad.hpp"
#include "gamepad_manager.hpp"
#include "gamepad_state.hpp"
#include "gamepad_input_processor.hpp"

#include <chrono>
#include <unordered_map>
#include <utility>

namespace Renderboi
{

using Axis = Window::Input::Gamepad::Axis;
using Button = Window::Input::Gamepad::Button;

GamepadManager::GamepadManager(const AbstractGamepadPtr gamepad, const unsigned int pollingRate, const bool enabled) :
    _gamepad(gamepad),
    _pollingRate(pollingRate),
    _pollingIntervalUs((int64_t)(1000000.f / pollingRate)),
    _enabled(enabled),
    _inputProcessor(std::make_shared<GamepadInputProcessor>()),
    _axisDeadZones(GetDefaultAxisDeadZones()),
    _stopRefreshing(false),
    _worker()
{

}

void GamepadManager::enable()
{
    if (!_enabled)
    {
        // TODO: run thread in charge of refreshing state

        _enabled = true;
    }
}

void GamepadManager::disable()
{
    if (_enabled)
    {
        // TODO: stop thread in charge of refreshing state

        _enabled = false;
    }
}

void GamepadManager::setEnabled(const bool enabled)
{
    enabled ? enable() : disable();
}

unsigned int GamepadManager::getPollingRate()
{
    return _pollingRate;
}

void GamepadManager::setPollingRate(const unsigned int pollingRate)
{
    if (_pollingRate != pollingRate)
    {
        // Stopping and restarting is better than having to synchronize the
        // polling rate, as it should only happen rarely.
        bool wasEnabled = _enabled;
        if (_enabled) disable();

        _pollingRate = pollingRate;
        _pollingIntervalUs = (int64_t)(1000000.f / pollingRate);
        if (wasEnabled) enable();
    }
}

const GamepadState& GamepadManager::getState()
{
    return _state;
}

bool GamepadManager::isPressed(const Button button)
{
    return _state[button];
}

float GamepadManager::getValue(const Axis axis)
{
    return _state[axis];
}

void GamepadManager::setAxisDeadZone(const Axis axis, std::pair<float, float> deadZone)
{
    _axisDeadZones[axis] = deadZone;
}

const std::unordered_map<Axis, std::pair<float, float>>& GamepadManager::GetDefaultAxisDeadZones()
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

GamepadInputProcessorPtr GamepadManager::getInputProcessor()
{
    return _inputProcessor;
}

void GamepadManager::setInputProcessor(const GamepadInputProcessorPtr inputProcessor)
{
    _inputProcessor = inputProcessor;
    if (_inputProcessor == nullptr)
    {
        _inputProcessor = std::make_shared<GamepadInputProcessor>();
    }
}

void GamepadManager::_refreshStateRoutine()
{
    std::chrono::time_point<std::chrono::system_clock> lastTimestamp = std::chrono::system_clock::now();
    while (!_stopRefreshing)
    {
        GamepadState lastState = _state;
        _state = _gamepad->getState();
        _compareAndFireEvents(lastState, _state);

        const std::chrono::time_point<std::chrono::system_clock> newTimestamp = std::chrono::system_clock::now();
        const std::chrono::duration<double> duration = newTimestamp - lastTimestamp;
        lastTimestamp = newTimestamp;
        const int64_t gap = _pollingIntervalUs - std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        std::this_thread::sleep_for(std::chrono::microseconds(gap));
    }
}

void GamepadManager::_compareAndFireEvents(const GamepadState& before, const GamepadState& after)
{
    using Window::Input::Action;

    if (before.A != after.A)
    {
        after.A
            ? _inputProcessor->processButton(Button::A, Action::Press)
            : _inputProcessor->processButton(Button::A, Action::Release);
    }

    if (before.B != after.B)
    {
        after.B
            ? _inputProcessor->processButton(Button::B, Action::Press)
            : _inputProcessor->processButton(Button::B, Action::Release);
    }

    if (before.X != after.X)
    {
        after.X
            ? _inputProcessor->processButton(Button::X, Action::Press)
            : _inputProcessor->processButton(Button::X, Action::Release);
    }

    if (before.Y != after.Y)
    {
        after.Y
            ? _inputProcessor->processButton(Button::Y, Action::Press)
            : _inputProcessor->processButton(Button::Y, Action::Release);
    }

    if (before.LeftBumper != after.LeftBumper)
    {
        after.LeftBumper
            ? _inputProcessor->processButton(Button::LeftBumper, Action::Press)
            : _inputProcessor->processButton(Button::LeftBumper, Action::Release);
    }

    if (before.RightBumper != after.RightBumper)
    {
        after.RightBumper
            ? _inputProcessor->processButton(Button::RightBumper, Action::Press)
            : _inputProcessor->processButton(Button::RightBumper, Action::Release);
    }

    if (before.DPadUp != after.DPadUp)
    {
        after.DPadUp
            ? _inputProcessor->processButton(Button::DPadUp, Action::Press)
            : _inputProcessor->processButton(Button::DPadUp, Action::Release);
    }

    if (before.DPadRight != after.DPadRight)
    {
        after.DPadRight
            ? _inputProcessor->processButton(Button::DPadRight, Action::Press)
            : _inputProcessor->processButton(Button::DPadRight, Action::Release);
    }

    if (before.DPadDown != after.DPadDown)
    {
        after.DPadDown
            ? _inputProcessor->processButton(Button::DPadDown, Action::Press)
            : _inputProcessor->processButton(Button::DPadDown, Action::Release);
    }

    if (before.DPadLeft != after.DPadLeft)
    {
        after.DPadLeft
            ? _inputProcessor->processButton(Button::DPadLeft, Action::Press)
            : _inputProcessor->processButton(Button::DPadLeft, Action::Release);
    }

    if (before.LeftThumb != after.LeftThumb)
    {
        after.LeftThumb
            ? _inputProcessor->processButton(Button::LeftThumb, Action::Press)
            : _inputProcessor->processButton(Button::LeftThumb, Action::Release);
    }

    if (before.RightThumb != after.RightThumb)
    {
        after.RightThumb
            ? _inputProcessor->processButton(Button::RightThumb, Action::Press)
            : _inputProcessor->processButton(Button::RightThumb, Action::Release);
    }

    if (before.Start != after.Start)
    {
        after.Start
            ? _inputProcessor->processButton(Button::Start, Action::Press)
            : _inputProcessor->processButton(Button::Start, Action::Release);
    }

    if (before.Select != after.Select)
    {
        after.Select
            ? _inputProcessor->processButton(Button::Select, Action::Press)
            : _inputProcessor->processButton(Button::Select, Action::Release);
    }

    if (before.Home != after.Home)
    {
        after.Home
            ? _inputProcessor->processButton(Button::Home, Action::Press)
            : _inputProcessor->processButton(Button::Home, Action::Release);
    }

    if (before.LeftX != after.LeftX)
    {
        _inputProcessor->processAxis(Axis::LeftX, after.LeftX);
    }
    
    if (before.LeftY != after.LeftY)
    {
        _inputProcessor->processAxis(Axis::LeftY, after.LeftY);
    }

    if (before.RightX != after.RightX)
    {
        _inputProcessor->processAxis(Axis::RightX, after.RightX);
    }

    if (before.RightY != after.RightY)
    {
        _inputProcessor->processAxis(Axis::RightY, after.RightY);
    }

    if (before.LeftTrigger != after.LeftTrigger)
    {
        _inputProcessor->processAxis(Axis::LeftTrigger, after.LeftTrigger);
    }

    if (before.RightTrigger != after.RightTrigger)
    {
        _inputProcessor->processAxis(Axis::RightTrigger, after.RightTrigger);
    }
}


}
