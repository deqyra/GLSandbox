#ifndef RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP
#define RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP

#include <atomic>
#include <map>

#include "gamepad.hpp"
#include "../enums.hpp"

namespace Renderboi
{

/// @brief Interface for an entity in charge of managing gamepad handles and 
/// forwarding input from the window.
class GamepadManager : public std::enable_shared_from_this<GamepadManager>
{
private:
    using Joystick = Window::Input::Joystick;

    /// @brief An instance count used for uniquely identifying instances.
    static unsigned int _count;

public:
    GamepadManager();

    /// @brief Callback for when a gamepad is connected on a slot.
    /// 
    /// @param slot Litteral describing the slot on which the gamepad was
    /// connected.
    virtual void gamepadConnected(Joystick slot) const = 0;

    /// @brief Callback for when a gamepad is disconnected from a slot.
    /// 
    /// @param slot Litteral describing the slot from which the gamepad was
    /// disconnected.
    virtual void gamepadDisconnected(Joystick slot) const = 0;

    /// @brief Get an array filled with litterals representing handles to
    /// present gamepads.
    ///
    /// @param unused Whether to return all present gamepads or only those not
    /// already in use by the window.
    ///
    /// @return An array filled with litterals representing handles to present 
    /// gamepads.
    virtual std::vector<Joystick> pollPresentGamepads(bool unused = true) const = 0;

    /// @brief Get a gamepad plugged into a certain slot. The gamepad can then
    /// be enabled to start receiving input from the main thread.
    ///
    /// @param slot Virtual slot on which to find the controller to manage.
    ///
    /// @return A pointer to the newly managed gamepad.
    ///
    /// @exception If a gamepad cannot be found on the provided slot, the 
    /// function will throw an std::runtime_error.
    virtual GamepadPtr getGamepad(Joystick slot) const = 0;

    /// @brief Enable polling the state for a gamepad. May be called from any
    /// thread.
    virtual void startGamepadPolling(Joystick slot) const = 0;

    /// @brief Disable polling the state for a gamepad. May be called from any
    /// thread.
    virtual void stopGamepadPolling(Joystick slat) const = 0;

    /// @brief Poll the state for gamepads. And forward it to gamepads.
    virtual void pollGamepadStates() const = 0;

    /// @brief Unique ID of the gamepad manager.
    const unsigned int id;

protected:
    /// @brief Method allowing inheriting classes to set the state of a gamepad
    /// without privileged access to it.
    ///
    /// @param gamepad Pointer to the gamepad instance whose state to set.
    /// @param state State to set within the gamepad.
    static void setGamepadState(GamepadPtr gamepad, GamepadState state);

    /// @brief Method allowing inheriting classes to notify a gamepad about its
    /// connection status without privileged access to it.
    ///
    /// @param gamepad Pointer to the gamepad instance to notify of its 
    /// connection status.
    static void setGamepadConnected(GamepadPtr gamepad);

    /// @brief Method allowing inheriting classes to notify a gamepad about its
    /// disconnection status without privileged access to it.
    ///
    /// @param gamepad Pointer to the gamepad instance to notify of its 
    /// disconnection status.
    static void setGamepadDisconnected(GamepadPtr gamepad);
};

using GamepadManagerPtr = std::shared_ptr<GamepadManager>;

}//namespace Renderboi

#endif//RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_MANAGER_HPP