#ifndef RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_HPP
#define RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_HPP

#include <memory>
#include <unordered_map>
#include <utility>

#include "../enums.hpp"
#include "gamepad_state.hpp"
#include "gamepad_input_processor.hpp"

namespace Renderboi
{

class GamepadManager;
using GamepadManagerPtr = std::shared_ptr<GamepadManager>;

/// @brief Proxy class to receive input from a XBox-like gamepad.
class Gamepad
{
public:
    using Axis = Window::Input::Gamepad::Axis;
    using Button = Window::Input::Gamepad::Button;

private:
    friend GamepadManager;

    /// @brief Current state of the gamepad (buttons and axes).
    GamepadState _state;

    /// @brief Whether or not to poll the gamepad state.
    bool _enabled;

    /// @brief Structure mapping a pair of floats between which axis input 
    /// should not be forwarded.
    std::unordered_map<Axis, std::pair<float, float>> _axisDeadZones;

    /// @brief Structure telling whether an axis value was in its dead zone in
    /// the previous state.
    std::unordered_map<Axis, bool> _axisWasInDeadZone;

    /// @brief Entity to which gamepad events should be forwarded.
    GamepadInputProcessorPtr _inputProcessor;

    /// @brief Pointer to the entity which will issue the calls to access the 
    /// concrete gamepad mechanisms.
    GamepadManagerPtr _manager;

    /// @brief To be called when the is connected.
    void _processConnected();

    /// @brief To be called when the gamepad is disconnected.
    void _processDisconnected();

    /// @brief Set the state of the gamepad, and compare it with the previous
    /// state to fire appropriate events to the gamepad input manager.
    ///
    /// @param newState New state to set the gamepad to.
    void _setNewState(const GamepadState newState);

    /// @brief Compare two gamepad states and fire events based on the 
    /// difference between both.
    ///
    /// @param before Gamepad state to compare the new one to.
    /// @param after Gamepad state to compare to the old one.
    void _compareAndFireEvents(const GamepadState& before, const GamepadState& after);

    /// @brief Tell whether a value is in the dead zone for a given axis.
    ///
    /// @param axis Litteral describing the axis whose dead zone to test.
    /// @param value Value to test against the axis dead zone.
    ///
    /// @return Whether or not the provided value is in the dead zone of the 
    /// provided axis.
    inline bool _valueIsInAxisDeadZone(const float& value, const Axis& axis) const;

public:
    /// @param manager Pointer to the gamepad manager which the gamepad will be linked to.
    Gamepad(const GamepadManagerPtr manager, const Window::Input::Joystick slot);

    /// @brief Enable polling the state of the gamepad.
    void enable();

    /// @brief Disable polling the state of the gamepad.
    void disable();

    /// @brief Whether to enable or disnable polling the state of the gamepad.
    void setEnabled(const bool enabled);

    /// @brief Get the current state of the gamepad managed by the instance.
    ///
    /// @return The current state of the gamepad managed by the instance.
    const GamepadState& getState() const;

    /// @brief Tells whether a button is currently pressed on the gamepad.
    ///
    /// @param button Litteral describing the button whose pressed state is to
    /// be checked.
    ///
    /// @return Whether the button is currently pressed on the gamepad.
    bool isPressed(const Button button) const;

    /// @brief Get the value currently assigned to an axis on the gamepad.
    ///
    /// @param axis Litteral describing the axis whose value is to be retrieved.
    ///
    /// @return The value currently assigned to the axis on the gamepad.
    float getValue(const Axis axis) const;

    /// @brief Get a pointer to the entity currently receiving gamepad events.
    ///
    /// @return A pointer to the entity currently receiving gamepad events.
    GamepadInputProcessorPtr getInputProcessor() const;

    /// @brief Set the pointer to the entity which will receive gamepad events.
    ///
    /// @param inputProcessor A pointer to the entity which will receive gamepad
    /// events.
    void setInputProcessor(const GamepadInputProcessorPtr inputProcessor);

    /// @brief Get the dead zone for an axis, a pair of values between which
    /// axis events are ignored and not being forwarded.
    ///
    /// @param axis Litteral describing which axis to get the dead zone for.
    ///
    /// @return The pair of values forming the axis dead zone.
    std::pair<float, float> getAxisDeadZone(const Axis axis) const;

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
    DefaultAxisDeadZones();

    /// @brief Get the rest values for gamepad axes.
    ///
    /// @return The rest values for gamepad axes.
    static const std::unordered_map<Axis, float>&
    AxisRestValues();

    /// @brief The slot on which the gamepad is plugged.
    const Window::Input::Joystick slot;
};

using GamepadPtr = std::shared_ptr<Gamepad>;

}//namespace Renderboi

#endif//RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_HPP