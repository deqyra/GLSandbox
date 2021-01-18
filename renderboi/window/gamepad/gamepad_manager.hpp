#ifndef RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP
#define RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP

#include "../enums.hpp"
#include "abstract_gamepad.hpp"
#include "gamepad_state.hpp"
#include "gamepad_input_processor.hpp"

#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>
#include <utility>

namespace Renderboi
{

/// @brief Handle input from a XBox-like gamepad.
class GamepadManager
{
public:
    using Axis = Window::Input::Gamepad::Axis;
    using Button = Window::Input::Gamepad::Button;

private:
    /// @brief Current state of the gamepad (buttons and axes).
    GamepadState _state;

    /// @brief Rate (in Hz) at which to poll the state of the gamepad.
    unsigned int _pollingRate;

    /// @brief Interval in µs at which the gamepad state will be polled.
    int64_t _pollingIntervalUs;

    /// @brief Whether or not to poll the gamepad state.
    bool _enabled;

    /// @brief Structure mapping a pair of floats between which axis input 
    /// should not be forwarded.
    std::unordered_map<Axis, std::pair<float, float>> _axisDeadZones;

    /// @brief Entity to which gamepad events should be forwarded.
    GamepadInputProcessorPtr _inputProcessor;

    /// @brief Entity in charge of implementing the concrete mechanisms to 
    /// access the gamepad state.
    AbstractGamepadPtr _gamepad;

    /// @brief Thread in charge of continuously refreshing the state of the 
    /// gamepad.
    std::thread _worker;

    /// @brief Flag telling a worker thread when to stop refreshing.
    std::atomic<bool> _stopRefreshing;

    /// @brief Routine to be run by the worker thread to refresh the gamepad state.
    void _refreshStateRoutine();

    /// @brief Compare two gamepad states and fire events based on the 
    /// difference between both.
    ///
    /// @param before Gamepad state to compare the new one to.
    /// @param after Gamepad state to compare to the old one.
    void _compareAndFireEvents(const GamepadState& before, const GamepadState& after);

public:
    /// @param gamepad Instance of the gamepad to manage.
    /// @param pollingRate Rate (in Hz) at which to poll the state of the gamepad.
    /// @param enabled Whether to enable polling the state of the gamepad.
    GamepadManager(const AbstractGamepadPtr gamepad, const unsigned int pollingRate, const bool enabled);

    /// @brief Enable polling the state of the gamepad.
    void enable();

    /// @brief Disable polling the state of the gamepad.
    void disable();

    /// @brief Whether to enable or disnable polling the state of the gamepad.
    void setEnabled(const bool enabled);

    /// @brief Get the rate at which the gamepad state is polled.
    unsigned int getPollingRate();

    /// @brief Get the rate at which the gamepad state is polled.
    ///
    /// @param pollingRate Rate at which the gamepad state should be polled.
    void setPollingRate(const unsigned int pollingRate);

    /// @brief Get the current state of the gamepad managed by the instance.
    ///
    /// @return The current state of the gamepad managed by the instance.
    const GamepadState& getState();

    /// @brief Tells whether a button is currently pressed on the gamepad.
    ///
    /// @param button Litteral describing the button whose pressed state is to
    /// be checked.
    ///
    /// @return Whether the button is currently pressed on the gamepad.
    bool isPressed(const Button button);

    /// @brief Get the value currently assigned to an axis on the gamepad.
    ///
    /// @param axis Litteral describing the axis whose value is to be retrieved.
    ///
    /// @return The value currently assigned to the axis on the gamepad.
    float getValue(const Axis axis);

    /// @brief Set the dead zone for an axis, a pair of values between which
    /// axis events are ignored and not being forwarded.
    ///
    /// @param axis Litteral describing the axis to set the dead zone for.
    /// @param deadZone Pair of floats defining the interval of values to ignore.
    void setAxisDeadZone(const Axis axis, std::pair<float, float> deadZone);

    /// @brief Get the default dead zones for gamepad axes.
    ///
    /// @return The default dead zones for gamepad axes.
    static const std::unordered_map<Axis, std::pair<float, float>>&
    GetDefaultAxisDeadZones();

    /// @brief Get a pointer to the entity currently receiving gamepad events.
    ///
    /// @return A pointer to the entity currently receiving gamepad events.
    GamepadInputProcessorPtr getInputProcessor();

    /// @brief Set the pointer to the entity which will receive gamepad events.
    ///
    /// @param inputProcessor A pointer to the entity which will receive gamepad
    /// events.
    void setInputProcessor(const GamepadInputProcessorPtr inputProcessor);
};

using GamepadManagerPtr = std::shared_ptr<GamepadManager>;

}

#endif//RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP