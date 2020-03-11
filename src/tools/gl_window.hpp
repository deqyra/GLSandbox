#ifndef TOOLS__GL_WINDOW_HPP
#define TOOLS__GL_WINDOW_HPP

#include <sstream>
#include <string>

#include "input_processor.hpp"

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLWindow
{
    public:
        GLWindow(GLFWwindow* window);

        // To be called upon a framebuffer resize event; forwards the call to the custom input processor
        void processFramebufferResize(GLFWwindow* window, int width, int height);
        // To be called upon a keyboard event; forwards the call to the custom input processor
        void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // To be called upon a mouse button event; forwards the call to the custom input processor
        void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // To be called upon a mouse cursor event; forwards the call to the custom input processor
        void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        // Register new input processor
        void registerInputProcessor(InputProcessorPtr inputProcessor);
        // Discard custom input processor
        void removeInputProcessor();

    private:
        // The GLFW window being managed
        GLFWwindow* _w;
        // Custom input processor to be registered at any time, providing custom callbacks
        InputProcessorPtr _inputProcessor;
};

#endif//TOOLS__GL_WINDOW_HPP
