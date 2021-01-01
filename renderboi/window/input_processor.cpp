#include "input_processor.hpp"

static std::vector<InputProcessor::EventType> listEventTypes();

// Initialize static stuff
unsigned int InputProcessor::_count = 0;
const std::vector<InputProcessor::EventType> InputProcessor::EventTypeList = listEventTypes();

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

std::vector<InputProcessor::EventType> listEventTypes()
{
    return {
        InputProcessor::EventType::FramebufferResize,
        InputProcessor::EventType::Keyboard,
        InputProcessor::EventType::MouseButton,
        InputProcessor::EventType::MouseCursor
    };
}