#include "glfw3_window_callbacks.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../gl_window.hpp"
#include "glfw3_adapter.hpp"

#include <iostream>

void globalGlfwFramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processFramebufferResize(width, height);
}

void globalGlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT) return;      // FIX ME IF REPEAT KEYS MUST BE HANDLED

    Window::Input::Key realKey = Window::GLFW3Adapter::getEnum<Window::Input::Key>(key);
    Window::Input::Action realAction = Window::GLFW3Adapter::getEnum<Window::Input::Action>(action);
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processKeyboard(realKey, scancode, realAction, mods);
}

void globalGlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_REPEAT) return;      // FIX ME IF REPEAT KEYS MUST BE HANDLED

    Window::Input::MouseButton realButton = Window::GLFW3Adapter::getEnum<Window::Input::MouseButton>(button);
    Window::Input::Action realAction = Window::GLFW3Adapter::getEnum<Window::Input::Action>(action);
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseButton(realButton, realAction, mods);
}

void globalGlfwMouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseCursor(xpos, ypos);
}

void globalGlfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW error: 0x" << std::hex << error << ", \"" << description << "\"" << std::endl;
}
