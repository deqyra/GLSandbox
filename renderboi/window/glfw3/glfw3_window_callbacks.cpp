#include "glfw3_window_callbacks.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../gl_window.hpp"
#include "glfw3_adapter.hpp"

#include <iostream>

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
