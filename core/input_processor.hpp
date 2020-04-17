#ifndef CORE__INPUT_PROCESSOR_HPP
#define CORE__INPUT_PROCESSOR_HPP

#include <string>
#include <memory>

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

// Callback manager to handle input from a GLFW window
class InputProcessor
{
    private:
        // Keeps track of how many InputProcessor objects were created (used as an ID system)
        static unsigned int _count;

    public:
        InputProcessor();

        // To be called upon a framebuffer resize event
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        // To be called upon a keyboard event
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // To be called upon a mouse button event
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // To be called upon a mouse cursor event
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        // Unique ID of the input processor
        const unsigned int id;
};

using InputProcessorPtr = std::shared_ptr<InputProcessor>;
using InputProcessorWPtr = std::weak_ptr<InputProcessor>;

#endif//CORE__INPUT_PROCESSOR_HPP
