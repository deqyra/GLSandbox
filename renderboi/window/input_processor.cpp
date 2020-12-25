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

void InputProcessor::processFramebufferResize(GLWindowPtr window, int width, int height)
{

}

void InputProcessor::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{

}

void InputProcessor::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{

}

void InputProcessor::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
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