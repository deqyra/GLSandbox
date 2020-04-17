#include "glfw_window_callbacks.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "gl_window.hpp"

void globalGlfwFramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processFramebufferResize(window, width, height);
}

void globalGlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processKeyboard(window, key, scancode, action, mods);
}

void globalGlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseButton(window, button, action, mods);
}

void globalGlfwMouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseCursor(window, xpos, ypos);
}
