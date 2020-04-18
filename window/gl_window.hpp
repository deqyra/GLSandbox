#ifndef WINDOW__GL_WINDOW_HPP
#define WINDOW__GL_WINDOW_HPP

#include <memory>
#include <sstream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "enums.hpp"
#include "../core/input_processor.hpp"
#include "../tools/enum_map.hpp"

namespace Window
{
// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLWindow : public std::enable_shared_from_this<GLWindow>
{
    public:
        GLWindow(GLFWwindow* window, std::string title);
        ~GLWindow();

        // Get a pointer to the managed window
        GLFWwindow* getWindow();

        // To be called upon a framebuffer resize event; forwards the call to the custom input processor
        void processFramebufferResize(int width, int height);
        // To be called upon a keyboard event; forwards the call to the custom input processor
        void processKeyboard(Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse button event; forwards the call to the custom input processor
        void processMouseButton(Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event; forwards the call to the custom input processor
        void processMouseCursor(double xpos, double ypos);

        // Register new input processor
        void registerInputProcessor(InputProcessorPtr inputProcessor);
        // Discard custom input processor
        void detachInputProcessor();

        // Whether the window was flagged for closing
        bool shouldClose();
        // Set the window closing flaf
        void setShouldClose(bool value);

        // Swap GLFW buffers
        void swapBuffers();
        // Poll GLFW events
        void pollEvents();

        // Set GLFW input mode
        void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value);

        // Get title of managed window
        std::string getTitle();
        // Get title of managed window
        void setTitle(std::string title);

        // Get cursor position in managed window
        void getCursorPos(double* x, double* y);

    private:
        // The GLFW window being managed
        GLFWwindow* _w;
        // Custom input processor to be registered at any time, providing custom callbacks
        InputProcessorPtr _inputProcessor;
        // Window title
        std::string _title;
};
}//namespace Window

using GLWindow = Window::GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

#endif//WINDOW__GL_WINDOW_HPP
