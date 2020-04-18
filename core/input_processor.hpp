#ifndef CORE__INPUT_PROCESSOR_HPP
#define CORE__INPUT_PROCESSOR_HPP

#include <string>
#include <memory>

#include "../window/enums.hpp"

namespace Window
{
    class GLWindow;
    using GLWindowPtr = std::shared_ptr<GLWindow>;
}

// Callback manager to handle input from a GLFW window
class InputProcessor
{
    private:
        // Keeps track of how many InputProcessor objects were created (used as an ID system)
        static unsigned int _count;

    public:
        InputProcessor();

        // To be called upon a framebuffer resize event
        virtual void processFramebufferResize(Window::GLWindowPtr window, int width, int height);
        // To be called upon a keyboard event
        virtual void processKeyboard(Window::GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse button event
        virtual void processMouseButton(Window::GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event
        virtual void processMouseCursor(Window::GLWindowPtr window, double xpos, double ypos);

        // Unique ID of the input processor
        const unsigned int id;
};

using InputProcessorPtr = std::shared_ptr<InputProcessor>;
using InputProcessorWPtr = std::weak_ptr<InputProcessor>;

#endif//CORE__INPUT_PROCESSOR_HPP
