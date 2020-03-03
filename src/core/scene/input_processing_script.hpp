#ifndef INPUT_PROCESSING_SCRIPT
#define INPUT_PROCESSING_SCRIPT

#include "script.hpp"
#include "../../tools/input_processor.hpp"

#include <memory>

#include "scene_object.hpp"
#include "../scene.hpp"

class InputProcessingScript : public Script, public InputProcessor, public std::enable_shared_from_this<InputProcessingScript>
{
    private:
        void removeInputProcessor();

    public:
        InputProcessingScript();
        virtual ~InputProcessingScript();

        // Callbacks to different events.
        virtual void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        virtual void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

        virtual void setSceneObject(SceneObjectWPtr sceneObject);
        virtual void update() = 0;
};

#endif//INPUT_PROCESSING_SCRIPT