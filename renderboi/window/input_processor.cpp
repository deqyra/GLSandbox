#include "input_processor.hpp"

namespace Renderboi::Window
{

// Initialize static stuff
unsigned int InputProcessor::_count = 0;

InputProcessor::InputProcessor() :
    id(_count++)                    // Generate ID from instance count
{

}

// Default callbacks do nothing

void InputProcessor::processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height)
{

}

void InputProcessor::processKeyboard(
    const GLWindowPtr window,
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
)
{

}

void InputProcessor::processMouseButton(
    const GLWindowPtr window,
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{

}

void InputProcessor::processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos)
{

}

}//namespace Renderboi::Window
