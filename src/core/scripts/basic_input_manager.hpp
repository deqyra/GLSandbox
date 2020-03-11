#ifndef CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP
#define CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP

#include "../scene/input_processing_script.hpp"

// Provide event callbacks associated with basic window management
class BasicInputManager : public InputProcessingScript
{
    public:
        BasicInputManager();

        // To be called upon a framebuffer resize event
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        // To be called upon a keyboard event
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // To be called upon a mouse button event
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // To be called upon a mouse cursor event
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);
};

// Return GLFW keyboard/mouse action name in a string
std::string glfwActionName(int action);
// Return GLFW keyboard key name in a string
std::string glfwKeyName(int key);
// Return GLFW mouse button name in a string
std::string glfwButtonName(int button);

#endif//CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP