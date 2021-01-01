#ifndef RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP
#define RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP

#include <memory>
#include <unordered_map>

#include <renderboi/window/input_processor.hpp>

/// @brief Forwards all input it receives to multiple subscribers.
class InputSplitter : public InputProcessor
{
private:
    unsigned int _subscriberRollingCount;

    /// @brief Array of subscribers to which input must be forwarded.
    std::unordered_map<unsigned int, InputProcessorPtr> _subscribers;

public:
    InputSplitter();

    /// @brief Register a new input processor. Use the returned subscription
    /// ID to detach it.
    ///
    /// @param inputProcessor Pointer to the new input processor.
    ///
    /// @return The subscription ID, used to unsubscribe the registered
    /// input processor.
    unsigned int registerInputProcessor(InputProcessorPtr inputProcessor);

    /// @brief Unsubscribe an input processor.
    ///
    /// @param subscriptionId ID returned when the input processor was first
    /// registered to the input splitter.
    void detachInputProcessor(unsigned int subscriptionId);

    /// @brief Unsubscribe all input processors from the input splitter.
    void detachAllInputProcessors();

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
    void processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height) override;

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
    void processKeyboard(
        const GLWindowPtr window,
        const Window::Input::Key key,
        const int scancode,
        const Window::Input::Action action,
        const int mods
    ) override;

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
    void processMouseButton(
        const GLWindowPtr window,
        const Window::Input::MouseButton button,
        const Window::Input::Action action,
        const int mods
    ) override;

    /// @brief Callback for a mouse cursor event.
    ///
    /// @param window Pointer to the GLWindow in which the event was
    /// triggered.
    /// @param xpos X coordinate of the new position of the mouse.
    /// @param ypos Y coordinate of the new position of the mouse.
    void processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos) override;
};

using InputSplitterPtr = std::shared_ptr<InputSplitter>;

#endif//RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP