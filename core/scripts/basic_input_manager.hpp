#ifndef CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP
#define CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP

#include "../scene/input_processing_script.hpp"

// Provide event callbacks associated with basic window management
class BasicInputManager : public InputProcessingScript
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Scripts are meant to be used only through pointers
        BasicInputManager(const BasicInputManager& other) = delete;
        BasicInputManager& operator=(const BasicInputManager& other) = delete;

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

        // Get a raw pointer to a new BasicInputManager instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual BasicInputManager* clone();
};

// Return GLFW keyboard/mouse action name in a string
std::string glfwActionName(int action);
// Return GLFW keyboard key name in a string
std::string glfwKeyName(int key);
// Return GLFW mouse button name in a string
std::string glfwButtonName(int button);

#endif//CORE__SCRIPTS__BASIC_INPUT_MANAGER_HPP