#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_GAMEPAD_MANAGER_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_GAMEPAD_MANAGER_HPP

#include <atomic>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"
#include "../gamepad/gamepad_manager.hpp"

namespace Renderboi
{

class GLFW3GamepadManager : public GamepadManager
{
private:
    using Joystick = Window::Input::Joystick;

    /// @brief Structure mapping managed gamepad to litterals describing the 
    /// slot which they are assigned to.
    mutable std::map<Window::Input::Joystick, GamepadPtr> _managedGamepads;

    /// @brief Structure mapping managed gamepad to whether or not polling for
    /// them is enabled.
    mutable std::map<Window::Input::Joystick, std::atomic<bool>> _enabledGamepads;

    /// @brief Get a Renderboi gamepad state from a GLFW3 gamepad state.
    ///
    /// @param glfwState GLFW gamepad state to convert to a Renderboi gamepad state.
    static GamepadState _GamepadStateFromGlfwGamepadState(const GLFWgamepadstate& glfwState);

public:
    GLFW3GamepadManager();

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from GamepadManager ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for when a gamepad is connected on a slot.
    /// 
    /// @param slot Litteral describing the slot on which the gamepad was
    /// connected.
    void gamepadConnected(Joystick slot) const override;

    /// @brief Callback for when a gamepad is disconnected from a slot.
    /// 
    /// @param slot Litteral describing the slot from which the gamepad was
    /// disconnected.
    void gamepadDisconnected(Joystick slot) const override;

    /// @brief Get an array filled with litterals representing handles to
    /// present gamepads.
    ///
    /// @param mustBeUnused Whether to return all present gamepads or only those
    /// not already in use by the window.
    ///
    /// @return An array filled with litterals representing handles to present 
    /// gamepads.
    std::vector<Joystick> pollPresentGamepads(bool mustBeUnused = true) const override;

    /// @brief Get a gamepad plugged into a certain slot. The gamepad can then
    /// be enabled to start receiving input from the main thread.
    ///
    /// @param slot Virtual slot on which to find the controller to manage.
    ///
    /// @return A pointer to the newly managed gamepad.
    ///
    /// @exception If a gamepad cannot be found on the provided slot, the 
    /// function will throw an std::runtime_error.
    GamepadPtr getGamepad(Joystick slot) override;

    /// @brief Enable polling the state for a gamepad. May be called from any
    /// thread.
    ///
    /// @param slot Virtual slot on which to find the controller to start polling.
    void startGamepadPolling(Joystick slot) const override;

    /// @brief Disable polling the state for a gamepad. May be called from any
    /// thread.
    ///
    /// @param slot Virtual slot on which to find the controller to stop polling.
    void stopGamepadPolling(Joystick slot) const override;

    /// @brief Poll the state for gamepads. And forward it to gamepads.
    void pollGamepadStates() const override;
};

}//namespace Renderboi


#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_GAMEPAD_MANAGER_HPP