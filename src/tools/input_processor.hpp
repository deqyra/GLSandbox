/**
    GLTest, input_processor.hpp
    Purpose: provide basic GLFW callback functionality. To be inherited from in order to customise any callback.

    @author François Brachais (deqyra)
    @version 1.0 14/09/2019
 */
#ifndef INPUT_PROCESSOR_HPP
#define INPUT_PROCESSOR_HPP

#include <string>
#include <memory>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

class InputProcessor
{
    private:
        static unsigned int _count;

    public:
        InputProcessor();
        // Callbacks to different events.
        virtual void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        virtual void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

        const unsigned int id;
};

// Helper functions to print button and key names.
std::string glfwActionName(int action);
std::string glfwKeyName(int key);
std::string glfwButtonName(int button);

using InputProcessorPtr = std::shared_ptr<InputProcessor>;
using InputProcessorWPtr = std::weak_ptr<InputProcessor>;

#endif//INPUT_PROCESSOR_HPP
