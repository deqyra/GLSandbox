#include "glfw3_utilities.hpp"

#include <iostream>
#include <unordered_map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"
#include "../gl_window.hpp"
#include "../gamepad/gamepad_manager.hpp"
#include "glfw3_adapter.hpp"
#include "glfw3_window.hpp"

namespace Renderboi
{

static std::unordered_map<unsigned int, GamepadManagerPtr>
_gamepadManagers = std::unordered_map<unsigned int, GamepadManagerPtr>();

static std::unordered_map<int, bool> _joystickIsGamepad = std::unordered_map<int, bool>();

void globalGlfwFramebufferResizeCallback(GLFWwindow* window, const int width, const int height)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processFramebufferResize(width, height);
}

void globalGlfwKeyboardCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
    if (action == GLFW_REPEAT) return;      // FIX ME IF REPEAT KEYS MUST BE HANDLED

    Window::Input::Key realKey = Window::GLFW3Adapter::getEnum<Window::Input::Key>(key);
    Window::Input::Action realAction = Window::GLFW3Adapter::getEnum<Window::Input::Action>(action);
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processKeyboard(realKey, scancode, realAction, mods);
}

void globalGlfwMouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
{
    if (action == GLFW_REPEAT) return;      // FIX ME IF REPEAT KEYS MUST BE HANDLED

    Window::Input::MouseButton realButton = Window::GLFW3Adapter::getEnum<Window::Input::MouseButton>(button);
    Window::Input::Action realAction = Window::GLFW3Adapter::getEnum<Window::Input::Action>(action);
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseButton(realButton, realAction, mods);
}

void globalGlfwMouseCursorCallback(GLFWwindow* window, const double xpos, const double ypos)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseCursor(xpos, ypos);
}

void globalGlfwErrorCallback(const int error, const char* description)
{
	std::cerr << "GLFW error: 0x" << std::hex << error << ", \"" << description << "\"" << std::endl;
}

void subscribeToGlfwJoystickStatus(GLWindowPtr window)
{
    GamepadManagerPtr manager = window->getGamepadManager();
    _gamepadManagers[manager->id] = manager;
}

void unsubscribeFromGlfwJoystickStatus(GLWindowPtr window)
{
    _gamepadManagers.erase(window->getGamepadManager()->id);
}

void globalGlfwJoystickCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        if (glfwJoystickIsGamepad(jid))
        {
            _joystickIsGamepad[jid] = true;

            for (auto it = _gamepadManagers.begin(); it != _gamepadManagers.end(); it++)
            {
                it->second->gamepadConnected(Window::GLFW3Adapter::getEnum<Window::Input::Joystick>(jid));
            }
        }
        // else { ... }
        // Joysticks not supported (yet).
    }
    else //if (event == GLFW_DISCONNECTED)
    {
        if (_joystickIsGamepad[jid])
        {
            for (auto it = _gamepadManagers.begin(); it != _gamepadManagers.end(); it++)
            {
                it->second->gamepadDisconnected(Window::GLFW3Adapter::getEnum<Window::Input::Joystick>(jid));
            }

            _joystickIsGamepad[jid] = false;
        }
        // else { ... }
        // Joysticks not supported (yet).
    }
}

void pollGamepads()
{
    for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST + 1; i++)
    {
        _joystickIsGamepad[i] = glfwJoystickIsGamepad(i);
    }
}

}//namespace Renderboi
