#include "input_processor.hpp"

// Initialize static stuff
unsigned int InputProcessor::_count = 0;

InputProcessor::InputProcessor() :
    id(_count++)                    // Generate ID from instance count
{

}

// Default callbacks do nothing

void InputProcessor::processFramebufferResize(Window::GLWindowPtr window, int width, int height)
{

}

void InputProcessor::processKeyboard(Window::GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{

}

void InputProcessor::processMouseButton(Window::GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{

}

void InputProcessor::processMouseCursor(Window::GLWindowPtr window, double xpos, double ypos)
{

}
