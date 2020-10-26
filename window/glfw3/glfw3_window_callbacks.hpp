#ifndef WINDOW__GLFW_WINDOW_CALLBACKS_HPP
#define WINDOW__GLFW_WINDOW_CALLBACKS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

/// @brief Callback for a framebuffer resize event. Will get the GLFW3Window instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param width New width (in pixels) of the framebuffer.
/// @param height New height (in pixels) of the framebuffer.
void globalGlfwFramebufferResizeCallback(GLFWwindow* window, int width, int height);

/// @brief Callback for a keyboard event. Will get the GLFW3Window instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param key Literal describing which key triggered the event.
/// @param scancode Scancode of the key which triggered the event. 
/// Platform-dependent, but consistent over time.
/// @param action Literal describing what action was performed on
/// the key which triggered the event.
/// @param mods Bit field describing which modifiers were enabled 
/// during the key event (Ctrl, Shift, etc).
void globalGlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/// @brief Callback for a mouse button event. Will get the GLFW3Window instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param button Literal describing which button triggered the
/// event.
/// @param action Literal describing what action was performed on
/// the button which triggered the event.
/// @param mods Bit field describing which modifiers were enabled 
/// during the button event (Ctrl, Shift, etc).
void globalGlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

/// @brief Callback for a mouse cursor event. Will get the GLFW3Window instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param xpos X coordinate of the new position of the mouse.
/// @param ypos Y coordinate of the new position of the mouse.
void globalGlfwMouseCursorCallback(GLFWwindow* window, double xpos, double ypos);


#endif//WINDOW__GLFW_WINDOW_CALLBACKS_HPP