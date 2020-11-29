#ifndef RENDERBOI__WINDOW__GL_WINDOW_HPP
#define RENDERBOI__WINDOW__GL_WINDOW_HPP

#include <memory>
#include <sstream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "enums.hpp"
#include "input_processor.hpp"

/// @brief Abstract window, supporting event callbacks through a custom 
/// InputProcessor.
class GLWindow : public std::enable_shared_from_this<GLWindow>
{
    public:
        /// @param title Title of the window.
        GLWindow(std::string title);
        ~GLWindow();

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        virtual void processFramebufferResize(int width, int height);

        /// @brief Callback for a keyboard event.
        ///
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        virtual void processKeyboard(Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse button event.
        ///
        /// @param button Literal describing which button triggered the
        /// event.
        /// @param action Literal describing what action was performed on
        /// the button which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the button event (Ctrl, Shift, etc).
        virtual void processMouseButton(Window::Input::MouseButton button, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(double xpos, double ypos);

        /// @brief Register an input processor to the window. Input events 
        /// will be forwarded to it.
        ///
        /// @param inputProcessor Pointer to the input processor to register
        /// to the window.
        ///
        /// @exception If the provided pointer is null, the function will 
        /// throw a std::runtime_error.
        virtual void registerInputProcessor(InputProcessorPtr inputProcessor);

        /// @brief Discard any registered custom input processor.
        virtual void detachInputProcessor();

        /// @brief Whether the window was flagged for closing.
        ///
        /// @return Whether or not the window was flagged for closing.
        virtual bool shouldClose() = 0;

        /// @brief Set the window closing flag.
        ///
        /// @param value Whether or not the window should be flagged for closing.
        virtual void setShouldClose(bool value) = 0;

        /// @brief Swap the front and back buffers of the window.
        virtual void swapBuffers() = 0;

        /// @brief Poll events recorded by the window.
        virtual void pollEvents() = 0;

        /// @brief Set the input mode of a certain target in the window.
        ///
        /// @param target Literal describing which aspect of the window whose
        /// input mode should be set.
        /// @param value Literal describing which input to set the target to.
        virtual void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value) = 0;

        /// @brief Get the title of the window.
        ///
        /// @return The title of the window.
        virtual std::string getTitle();

        /// @brief Set the title of the window.
        ///
        /// @return The title of the window.
        virtual void setTitle(std::string title) = 0;
        
        /// @brief Get the aspect ratio of the framebuffer used by the window.
        ///
        /// @return The aspect ratio of the framebuffer used by the window.
        virtual float getAspectRatio() = 0;

        /// @brief Get position of the mouse cursor in the window.
        ///
        /// @param x [Output parameter] The X coordinate of the cursor.
        /// @param y [Output parameter] The Y coordinate of the cursor.
        virtual void getCursorPos(double* x, double* y) = 0;

    protected:
        /// @brief Default input processor of all windows.
        static InputProcessorPtr _defaultInputProcessor;

        /// @brief Custom input processor to be registered at any point, 
        /// providing custom callbacks.
        InputProcessorPtr _inputProcessor;

        /// @brief Title of the window.
        std::string _title;

        /// @brief Width of the window.
        int _width;

        /// @brief Height of the window.
        int _height;
};

using GLWindowPtr = std::shared_ptr<GLWindow>;

#endif//RENDERBOI__WINDOW__GL_WINDOW_HPP
