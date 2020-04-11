#ifndef CORE__SCENE__INPUT_PROCESSING_SCRIPT
#define CORE__SCENE__INPUT_PROCESSING_SCRIPT

#include "script.hpp"
#include "../../tools/input_processor.hpp"

#include <memory>

#include "scene_object.hpp"

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
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        // To be called upon a keyboard event
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // To be called upon a mouse button event
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // To be called upon a mouse cursor event
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        // To be called once per frame
        virtual void update(float timeElapsed);

        // Get a raw pointer to a new InputProcessingScript instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual InputProcessingScript* clone() = 0;
};

using InputProcessingScriptPtr = std::shared_ptr<InputProcessingScript>;
using InputProcessingScriptWPtr = std::weak_ptr<InputProcessingScript>;

#endif//CORE__SCENE__INPUT_PROCESSING_SCRIPT