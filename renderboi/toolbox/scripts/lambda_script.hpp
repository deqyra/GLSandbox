#ifndef RENDERBOI__TOOLBOX__SCRIPTS__LAMBDA_SCRIPT_HPP
#define RENDERBOI__TOOLBOX__SCRIPTS__LAMBDA_SCRIPT_HPP

#include "../input_processing_script.hpp"

#include <functional>

/// @brief Provides event callbacks which can be set on the fly using lambdas.
class LambdaScript : public InputProcessingScript
{
    private:
        LambdaScript(const LambdaScript& other) = delete;
        LambdaScript& operator=(const LambdaScript& other) = delete;

    public:
        using Update_CB = std::function<void(float)>;
        using FramebufferResize_CB = std::function<void(GLWindowPtr, int, int)>;
        using Keyboard_CB = std::function<void(GLWindowPtr, Window::Input::Key, int, Window::Input::Action, int)>;
        using MouseButton_CB = std::function<void(GLWindowPtr, Window::Input::MouseButton, Window::Input::Action, int)>;
        using MouseCursor_CB = std::function<void(GLWindowPtr, double, double)>;

        LambdaScript();

        /// @brief The callback to call upon a frame update.
        Update_CB updateCallback;

        /// @brief The callback to call upon a framebuffer resize event.
        FramebufferResize_CB framebufferResizeCallback;

        /// @brief The callback to call upon a keyboard event.
        Keyboard_CB keyboardCallback;

        /// @brief The callback to call upon a mouse button event.
        MouseButton_CB mouseButtonCallback;

        /// @brief The callback to call upon a mouse cursor event.
        MouseCursor_CB mouseCursorCallback;
        
        /// @brief Make the lambda run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(float timeElapsed);

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

        /// @brief Get a raw pointer to a new lambda script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual LambdaScript* clone();
};

#endif//RENDERBOI__TOOLBOX__SCRIPTS__LAMBDA_SCRIPT_HPP