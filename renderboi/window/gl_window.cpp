#include "gl_window.hpp"

#include <iostream>
#include <string>

#include "input_processor.hpp"

namespace Renderboi::Window
{

const InputProcessorPtr GLWindow::_DefaultInputProcessor = std::make_shared<InputProcessor>();

GLWindow::GLWindow(std::string title) :
    _inputProcessor(_DefaultInputProcessor),
    _title(title),
    _exitSignaled(false),
    _gamepadManager(nullptr)
{

}

GLWindow::~GLWindow()
{

}

void GLWindow::processFramebufferResize(const int width, const int height)
{
    _inputProcessor->processFramebufferResize(shared_from_this(), (unsigned int) width, (unsigned int) height);
}

void GLWindow::processKeyboard(
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
)
{
    _inputProcessor->processKeyboard(shared_from_this(), key, scancode, action, mods);
}

void GLWindow::processMouseButton(
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{
    _inputProcessor->processMouseButton(shared_from_this(), button, action, mods);
}

void GLWindow::processMouseCursor(const double xpos, const double ypos)
{
    _inputProcessor->processMouseCursor(shared_from_this(), xpos, ypos);
}

void GLWindow::registerInputProcessor(const InputProcessorPtr inputProcessor)
{
    if (!_inputProcessor)
    {
        throw std::runtime_error("GLWindow: cannot register null input processor.");
    }
    
    _inputProcessor = inputProcessor;
}

void GLWindow::detachInputProcessor()
{
    _inputProcessor = _DefaultInputProcessor;
}

std::string GLWindow::getTitle() const
{
    return _title;
}

GLContextClientPtr GLWindow::getGlContextClient() const
{
    return _glContextClient;
}

void GLWindow::pollAllEvents()
{
    pollEvents();

    _gamepadManager->refreshGamepadStatuses();
    _gamepadManager->pollGamepadStates();
}

void GLWindow::startPollingLoop()
{
    _exitSignaled = false;
    while (!_exitSignaled)
    {
        pollAllEvents();
    }
}

void GLWindow::signalExit(bool value)
{
    _exitSignaled = value;
}

bool GLWindow::exitSignaled()
{
    return _exitSignaled;
}

GamepadManagerPtr GLWindow::getGamepadManager()
{
    return _gamepadManager;
}


}//namespace Renderboi::Window
