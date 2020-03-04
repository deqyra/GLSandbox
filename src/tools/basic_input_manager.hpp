#ifndef BASIC_INPUT_MANAGER_HPP
#define BASIC_INPUT_MANAGER_HPP

#include "../core/scene/input_processing_script.hpp"

class BasicInputManager : public InputProcessingScript
{
    public:
        BasicInputManager();

        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);
};

// Helper functions to print button and key names.
std::string glfwActionName(int action);
std::string glfwKeyName(int key);
std::string glfwButtonName(int button);

#endif//BASIC_INPUT_MANAGER_HPP