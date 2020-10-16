#ifndef CORE__SCENE__INPUT_PROCESSING_SCRIPT
#define CORE__SCENE__INPUT_PROCESSING_SCRIPT

#include "script.hpp"
#include "../input_processor.hpp"

#include <memory>

#include "scene_object.hpp"
#include "../../window/gl_window.hpp"

/// @brief A special flavor of Script, also able to process input from the app.
class InputProcessingScript : public Script, public InputProcessor, public std::enable_shared_from_this<InputProcessingScript>
{
    private:
        InputProcessingScript(const InputProcessingScript& other) = delete;
        InputProcessingScript& operator=(const InputProcessingScript& other) = delete;

    public:
        InputProcessingScript();

        virtual ~InputProcessingScript();

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
        /// during the button event (Ctrl, Shift, etc).
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

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(float timeElapsed);

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual InputProcessingScript* clone() = 0;
};

using InputProcessingScriptPtr = std::shared_ptr<InputProcessingScript>;
using InputProcessingScriptWPtr = std::weak_ptr<InputProcessingScript>;

#endif//CORE__SCENE__INPUT_PROCESSING_SCRIPT