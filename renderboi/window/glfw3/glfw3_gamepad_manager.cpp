#include "glfw3_gamepad_manager.hpp"

#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <renderboi/utilities/to_string.hpp>

#include "../enums.hpp"
#include "glfw3_adapter.hpp"

namespace Renderboi
{

GLFW3GamepadManager::GLFW3GamepadManager() :
    GamepadManager(),
    _managedGamepads(),
    _enabledGamepads()
{
    using Window::Input::Joysticks;
    for (auto it = Joysticks.cbegin(); it != Joysticks.cend(); it++)
    {
        _enabledGamepads[*it] = false;
    }
}

void GLFW3GamepadManager::gamepadConnected(Joystick slot) const
{
    if (_managedGamepads.contains(slot) && _enabledGamepads.at(slot))
    {
        GamepadManager::setGamepadConnected(_managedGamepads.at(slot));
    }
}

void GLFW3GamepadManager::gamepadDisconnected(Joystick slot) const
{
    if (_managedGamepads.contains(slot) && _enabledGamepads.at(slot))
    {
        GamepadManager::setGamepadDisconnected(_managedGamepads.at(slot));
    }
}

std::vector<Window::Input::Joystick> GLFW3GamepadManager::pollPresentGamepads(bool mustBeUnused) const
{
    using Window::Input::Joysticks;

    std::vector<Window::Input::Joystick> result;
    for (auto it = Joysticks.cbegin(); it != Joysticks.cend(); it++)
    {
        int jid = Window::GLFW3Adapter::getValue(*it);

        bool gamepad = glfwJoystickIsGamepad(jid);
        if (glfwJoystickIsGamepad(jid) && !(mustBeUnused && _managedGamepads.contains(*it)))
        {
            result.push_back(*it);
        }
    }

    return result;
}

GamepadPtr GLFW3GamepadManager::getGamepad(Window::Input::Joystick slot)
{
    if (_managedGamepads.contains(slot)) return _managedGamepads.at(slot);

    int jid = Window::GLFW3Adapter::getValue(slot);
    if (!glfwJoystickIsGamepad(jid))
    {
        throw std::runtime_error("GLFW3GamepadManager: no gamepad on slot " + to_string(slot) + ".");
    }

    std::string gamepadName(glfwGetGamepadName(jid));
    _managedGamepads[slot] = GamepadManager::createGamepad(shared_from_this(), slot, gamepadName);
    return _managedGamepads[slot];
}

void GLFW3GamepadManager::startGamepadPolling(Window::Input::Joystick slot) const
{
    _enabledGamepads[slot] = true;
}

void GLFW3GamepadManager::stopGamepadPolling(Window::Input::Joystick slot) const
{
    _enabledGamepads[slot] = false;
}

void GLFW3GamepadManager::pollGamepadStates() const
{
    for (auto it = _managedGamepads.cbegin(); it != _managedGamepads.cend(); it++)
    {
        if (!_enabledGamepads.at(it->first)) continue;

        int jid = Window::GLFW3Adapter::getValue(it->first);
        GLFWgamepadstate glfwState;
        glfwGetGamepadState(jid, &glfwState);
        GamepadManager::setGamepadState(it->second, _GamepadStateFromGlfwGamepadState(glfwState));
    }
}

GamepadState GLFW3GamepadManager::_GamepadStateFromGlfwGamepadState(const GLFWgamepadstate& glfwState)
{
    GamepadState state;
    state.A             = glfwState.buttons[GLFW_GAMEPAD_BUTTON_A];
    state.B             = glfwState.buttons[GLFW_GAMEPAD_BUTTON_B];
    state.X             = glfwState.buttons[GLFW_GAMEPAD_BUTTON_X];
    state.Y             = glfwState.buttons[GLFW_GAMEPAD_BUTTON_Y];
    state.LeftBumper    = glfwState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
    state.RightBumper   = glfwState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
    state.Select        = glfwState.buttons[GLFW_GAMEPAD_BUTTON_BACK];
    state.Start         = glfwState.buttons[GLFW_GAMEPAD_BUTTON_START];
    state.Home          = glfwState.buttons[GLFW_GAMEPAD_BUTTON_GUIDE];
    state.LeftThumb     = glfwState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB];
    state.RightThumb    = glfwState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB];
    state.DPadUp        = glfwState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
    state.DPadRight     = glfwState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
    state.DPadDown      = glfwState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
    state.DPadLeft      = glfwState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
    state.DPad          = state.DPadUp    ? GamepadState::DPadDirection::Up
                        : state.DPadRight ? GamepadState::DPadDirection::Right
                        : state.DPadDown  ? GamepadState::DPadDirection::Down
                        : state.DPadLeft  ? GamepadState::DPadDirection::Left
                        : GamepadState::DPadDirection::None;
                        
    state.LeftX         = glfwState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
    state.LeftY         = glfwState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
    state.RightX        = glfwState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    state.RightY        = glfwState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
    state.LeftTrigger   = glfwState.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    state.RightTrigger  = glfwState.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

    return state;
}

}
