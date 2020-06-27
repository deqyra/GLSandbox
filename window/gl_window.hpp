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

namespace Window
{
// To be attached to a concrete window object (provided by a backend), providing event callback functionality through a custom InputProcessor
class GLWindow : public std::enable_shared_from_this<GLWindow>
{
    public:
        GLWindow(std::string title);
        ~GLWindow();

        // To be called upon a framebuffer resize event; forwards the call to the custom input processor
        virtual void processFramebufferResize(int width, int height);
        // To be called upon a keyboard event; forwards the call to the custom input processor
        virtual void processKeyboard(Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse button event; forwards the call to the custom input processor
        virtual void processMouseButton(Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event; forwards the call to the custom input processor
        virtual void processMouseCursor(double xpos, double ypos);

        // Register new input processor
        virtual void registerInputProcessor(InputProcessorPtr inputProcessor);
        // Discard custom input processor
        virtual void detachInputProcessor();

        // Whether the window was flagged for closing
        virtual bool shouldClose() = 0;
        // Set the window closing flag
        virtual void setShouldClose(bool value) = 0;

        // Swap buffers
        virtual void swapBuffers() = 0;
        // Poll events
        virtual void pollEvents() = 0;

        // Set input mode
        virtual void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value) = 0;

        // Get title of managed window
        virtual std::string getTitle();
        // Set title of managed window
        virtual void setTitle(std::string title) = 0;

        // Get cursor position in managed window
        virtual void getCursorPos(double* x, double* y) = 0;

    private:
        // Custom input processor to be registered at any time, providing custom callbacks
        InputProcessorPtr _inputProcessor;
        // Window title
        std::string _title;
};


}//namespace Window

using GLWindow = Window::GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

#endif//WINDOW__GL_WINDOW_HPP
