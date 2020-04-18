#ifndef CORE__SCENE__INPUT_PROCESSING_SCRIPT
#define CORE__SCENE__INPUT_PROCESSING_SCRIPT

#include "script.hpp"
#include "../input_processor.hpp"

#include <memory>

#include "scene_object.hpp"
#include "../../window/gl_window.hpp"

// A special flavor of Script, also able to process input from a GLFW window
class InputProcessingScript : public Script, public InputProcessor, public std::enable_shared_from_this<InputProcessingScript>
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Scripts are meant to be used only through pointers
        InputProcessingScript(const InputProcessingScript& other) = delete;
        InputProcessingScript& operator=(const InputProcessingScript& other) = delete;

    public:
        InputProcessingScript();
        virtual ~InputProcessingScript();

        // To be called upon a framebuffer resize event
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);
        // To be called upon a keyboard event
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse button event
        virtual void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        // To be called once per frame
        virtual void update(float timeElapsed);

        // Get a raw pointer to a new InputProcessingScript instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual InputProcessingScript* clone() = 0;
};

using InputProcessingScriptPtr = std::shared_ptr<InputProcessingScript>;
using InputProcessingScriptWPtr = std::weak_ptr<InputProcessingScript>;

#endif//CORE__SCENE__INPUT_PROCESSING_SCRIPT