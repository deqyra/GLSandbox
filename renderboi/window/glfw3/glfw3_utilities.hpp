#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_UTILITIES_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_UTILITIES_HPP

#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../gl_window.hpp"

namespace Renderboi::Window::GLFW3Utilities
{

/// @brief Callback for a framebuffer resize event. Will get the GLWindow instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param width New width (in pixels) of the framebuffer.
/// @param height New height (in pixels) of the framebuffer.
void globalGlfwFramebufferResizeCallback(GLFWwindow* window, const int width, const int height);

/// @brief Callback for a keyboard event. Will get the GLWindow instance
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
void globalGlfwKeyboardCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);

/// @brief Callback for a mouse button event. Will get the GLWindow instance
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
void globalGlfwMouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods);

/// @brief Callback for a mouse cursor event. Will get the GLWindow instance
/// corresponding to the window which received the event, from its user pointer,
/// and will forward the event to it.
///
/// @param window Pointer to the GLFWwindow struct on which the event took
/// place.
/// @param xpos X coordinate of the new position of the mouse.
/// @param ypos Y coordinate of the new position of the mouse.
void globalGlfwMouseCursorCallback(GLFWwindow* window, const double xpos, const double ypos);

/// @brief Callback for error-reporting by GLFW.
///
/// @param error Code of an error which just occurred.
/// @param description C-string containing a description of the error which just
/// occurred.
void globalGlfwErrorCallback(const int error, const char* description);

/// @brief Subscribe a window to the event callback for when a joystick is 
/// connected.
///
/// @param window Pointer to the window to subscribe.
void subscribeToGlfwJoystickStatus(GLWindowPtr window);

/// @brief Unsubscribe a window from the event callback for when a joystick is 
/// connected.
///
/// @param window Pointer to the window to unsubscribe.
void unsubscribeFromGlfwJoystickStatus(GLWindowPtr window);

/// @brief Callback for a joystick status event. The event will be forwarded to
/// the gamepad managers of all windows which were subscribed through 
/// subscribeToGlfwJoystickStatus().
/// CAUTION : this function will be called by GLFW in a separate thread. However,
/// in the case of a connection event, actually checking the status of a 
/// joystick (to know whether it is a gamepad or not) requires to call GLFW 
/// functions which only work when called from the main thread. Since this is 
/// not the case here, what this callback actually does is raise a flag to check
/// upon joystick statuses. The actual processing is then deferred to the main
/// thread of the application, which should call 
/// GLFW3GamepadManager::refreshGamepadStatuses() at some point (typically 
/// done through calling GLWindow::startEventProcessingLoop()). Only then will 
/// the connection events be fired to registered gamepad managers.
/// Gamepad disconnection events are not affected by this.
///
/// @param jid ID of the joystick which the connection / disconnection event 
/// relates to.
/// @param event Value describing whether the joystick was connected or 
/// disconnected. 
void globalGlfwJoystickCallback(int jid, int event);

/// @brief Poll all gamepads and initialize internal structures to keep track of
/// those.
void initGamepadStatuses();

}//namespace Renderboi::Window::GLFW3Utilities

#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_UTILITIES_HPP