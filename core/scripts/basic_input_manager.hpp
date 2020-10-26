#ifndef CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP
#define CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP

#include "../scene/input_processing_script.hpp"

/// @brief Provides event callbacks associated with basic window management.
class BasicInputManager : public InputProcessingScript
{
    private:
        BasicInputManager(const BasicInputManager& other) = delete;
        BasicInputManager& operator=(const BasicInputManager& other) = delete;

    public:
        BasicInputManager();

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse button event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param button Literal describing which button triggered the
        /// event.
        /// @param action Literal describing what action was performed on
        /// the button which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the button event (Ctrl, Shift, etc).
        virtual void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        /// @brief Get a raw pointer to a new basic input script instance cloned
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual BasicInputManager* clone();
};

/// @brief Return GLFW keyboard/mouse action name in a string.
///
/// @param action Literal describing an action which was performed on a key or 
/// button.
///
/// @return A string describing the action represented by the literal.
std::string glfwActionName(Window::Input::Action action);

/// @brief Return GLFW keyboard key name in a string.
///
/// @param key Literal describing a keyboard key.
///
/// @return A string describing the key represented by the literal.
std::string glfwKeyName(Window::Input::Key key);

/// @brief Return GLFW mouse button name in a string.
///
/// @param button Literal describing a mouse button.
///
/// @return A string describing the button represented by the literal.
std::string glfwButtonName(Window::Input::MouseButton button);

#endif//CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP