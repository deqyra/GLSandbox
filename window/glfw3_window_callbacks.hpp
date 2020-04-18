#ifndef WINDOW__GLFW_WINDOW_CALLBACKS_HPP
#define WINDOW__GLFW_WINDOW_CALLBACKS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

// Fetch the GLWindow object associated with the GLFWwindow object, and call the custom framebuffer resize callback
void globalGlfwFramebufferResizeCallback(GLFWwindow* window, int width, int height);
// Fetch the GLWindow object associated with the GLFWwindow object, and call the custom keyboardcallback
void globalGlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
// Fetch the GLWindow object associated with the GLFWwindow object, and call the custom mouse button callback
void globalGlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
// Fetch the GLWindow object associated with the GLFWwindow object, and call the custom mouse cursor callback
void globalGlfwMouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

#endif//WINDOW__GLFW_WINDOW_CALLBACKS_HPP