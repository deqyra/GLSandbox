/**
    GLTest, gl_window.hpp
    Purpose: GLFW window handler. Encapsulates callbacks.

    @author François Brachais (deqyra)
    @version 1.0 19/06/2019
 */
#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include <sstream>
#include <string>

#include "input_processor.hpp"

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

class GLWindow
{
    public:
        GLWindow(GLFWwindow* window);

        // Callbacks to different events.
        // In all of these, the call is forwarded to the custom input processor if any, or to the base input processor otherwise.
        void processFramebufferResize(GLFWwindow* window, int width, int height);
        void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        // Custom input processor registration.
        void registerInputProcessor(InputProcessorPtr inputProcessor);
        void removeInputProcessor();

    private:
        GLFWwindow* _w;                         // Handled GLFW window.
        InputProcessorPtr _inputProcessor;// Custom input processor to be registered at any time, providing overriden, custom callbacks.
};

#endif//GL_WINDOW_HPP
