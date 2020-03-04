#ifndef INPUT_PROCESSING_SCRIPT
#define INPUT_PROCESSING_SCRIPT

#include "script.hpp"
#include "../../tools/input_processor.hpp"

#include <memory>

#include "scene_object.hpp"

class InputProcessingScript : public Script, public InputProcessor, public std::enable_shared_from_this<InputProcessingScript>
{
    protected:
        void registerInputProcessor();
        void removeInputProcessor();

    public:
        InputProcessingScript();
        virtual ~InputProcessingScript();

        // Callbacks to different events.
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        virtual void update(float timeElapsed);

        virtual void setSceneObject(SceneObjectWPtr sceneObject);
};

using InputProcessingScriptPtr = std::shared_ptr<InputProcessingScript>;
using InputProcessingScriptWPtr = std::weak_ptr<InputProcessingScript>;

#endif//INPUT_PROCESSING_SCRIPT