#include "input_logger.hpp"

#include <bitset>

#include <renderboi/window/gl_window.hpp>

InputLogger::InputLogger(std::ostream& outputStream) :
    _loggingStatus(),
    _outputStream(outputStream)
{
    const std::vector<EventType>& typeList = InputProcessor::EventTypeList;
    for (auto it = typeList.begin(); it != typeList.end(); it++)
    {
        _loggingStatus[*it] = true;
    }
}

void InputLogger::enableEventLog(InputProcessor::EventType eventType)
{
    _loggingStatus[eventType] = true;
}

void InputLogger::disableEventLog(InputProcessor::EventType eventType)
{
    _loggingStatus[eventType] = false;
}

void InputLogger::setEventLoggingStatus(InputProcessor::EventType eventType, bool enable)
{
    _loggingStatus[eventType] = enable;
}

void InputLogger::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    if (!_loggingStatus[InputProcessor::EventType::FramebufferResize]) return;

    _outputStream << "Event on window \"" << window->getTitle() << "\": framebuffer resized.\n"
                     "New dimensions: w=" << width << ";h=" << height << ".\n" << std::endl;
}

void InputLogger::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    if (!_loggingStatus[InputProcessor::EventType::Keyboard]) return;

    std::bitset<8> bits(mods);

    _outputStream << "Event on window \"" << window->getTitle() << "\": keyboard.\n"
                     "Key: " << std::to_string(key) << " (scancode: " << std::hex << scancode << std::dec << ")\n"
                     "Action: " << std::to_string(action) << "\n"
                     "Mod flags: " << bits << "\n" << std::endl;
}

void InputLogger::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    if (!_loggingStatus[InputProcessor::EventType::MouseButton]) return;

    std::bitset<8> bits(mods);

    _outputStream << "Event on window \"" << window->getTitle() << "\": mouse button.\n"
                     "Button: " << std::to_string(button) << "\n"
                     "Action: " << std::to_string(action) << "\n"
                     "Mod flags: " << bits << "\n" << std::endl;
}

void InputLogger::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{
    if (!_loggingStatus[InputProcessor::EventType::MouseCursor]) return;

    _outputStream << "Event on window \"" << window->getTitle() << "\": mouse moved.\n"
                     "New position: x=" << xpos << ";y=" << ypos << ".\n" << std::endl;
}
