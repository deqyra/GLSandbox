#ifndef RENDERBOI__TOOLBOX__INPUT_LOGGER_HPP
#define RENDERBOI__TOOLBOX__INPUT_LOGGER_HPP

#include <iostream>
#include <memory>
#include <unordered_map>

#include <renderboi/window/input_processor.hpp>

class InputLogger : public InputProcessor
{
    private:
        /// @brief Structure telling whether events of a certain type should be
        /// logged.
        std::unordered_map<InputProcessor::EventType, bool> _loggingStatus;

        /// @brief Stream to which events should be logged. Very shady and prone
        /// to errors but will do for now.
        std::ostream& _outputStream;

    public:
        /// @param outputStream Stream to which events should be logged.
        InputLogger(std::ostream& outputStream = std::cout);

        /// @brief Enables logging for a certain type of input event.
        ///
        /// @param eventType Litteral describing the type of the event for which
        /// to enable logging.
        void enableEventLog(InputProcessor::EventType eventType);

        /// @brief Disables logging for a certain type of input event.
        ///
        /// @param eventType Litteral describing the type of the event for which
        /// to disable logging.
        void disableEventLog(InputProcessor::EventType eventType);

        /// @brief Set the logging status for a certain type of input event: 
        /// enabled or disabled.
        ///
        /// @param eventType Litteral describing the type of the event for which
        /// to set the logging status.
        /// @param enable Whether to enable or disable logging for the event 
        /// type.
        void setEventLoggingStatus(InputProcessor::EventType eventType, bool enable);

        //////////////////////////////////////////////
        ///                                        ///
        /// Methods overridden from InputProcessor ///
        ///                                        ///
        //////////////////////////////////////////////

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        void processFramebufferResize(GLWindowPtr window, int width, int height);

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
        void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

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
        void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        void processMouseCursor(GLWindowPtr window, double xpos, double ypos);
};

using InputLoggerPtr = std::shared_ptr<InputLogger>;

#endif//RENDERBOI__TOOLBOX__INPUT_LOGGER_HPP