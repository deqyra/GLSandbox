#include "glfw3_utilities.hpp"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"
#include "../gl_window.hpp"
#include "../gamepad/gamepad_manager.hpp"
#include "glfw3_adapter.hpp"
#include "glfw3_window.hpp"
#include "glfw3_gamepad_manager.hpp"

namespace Renderboi::Window::GLFW3Utilities
{

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
    GLFW3GamepadManager::_GamepadManagers[manager->id] = manager;
}

void unsubscribeFromGlfwJoystickStatus(GLWindowPtr window)
{
    GLFW3GamepadManager::_GamepadManagers.erase(window->getGamepadManager()->id);
}

void globalGlfwJoystickCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        GLFW3GamepadManager::_PresentJoysticks[jid] = true;
        GLFW3GamepadManager::_PresentJoysticks[jid] = false;
        // No mutex lock because the flag only needs to be atomic-written to,
        // regardless of the value it held before.
        GLFW3GamepadManager::_JoystickStatusRefreshFlag = true;
    }
    else //if (event == GLFW_DISCONNECTED)
    {
        if (GLFW3GamepadManager::_PresentGamepads[jid])
        {
            for (const auto& [_, manager] : GLFW3GamepadManager::_GamepadManagers)
            {
                manager->gamepadDisconnected(Window::GLFW3Adapter::getEnum<Window::Input::Joystick>(jid));
            }

            GLFW3GamepadManager::_PresentGamepads[jid] = false;
        }
        // else { ... }
        // Joysticks not supported (yet).

        GLFW3GamepadManager::_PresentJoysticks[jid] = false;
    }
}

void initGamepadStatuses()
{
    for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST + 1; i++)
    {
        GLFW3GamepadManager::_PresentJoysticks[i] = glfwJoystickPresent(i);
        GLFW3GamepadManager::_PresentGamepads[i] = glfwJoystickIsGamepad(i);
    }
}

}//namespace Renderboi::GLFW3Utilities
