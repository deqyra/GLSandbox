#include "gl_window.hpp"

#include <iostream>
#include <string>

#include "glfw3/glfw3_adapter.hpp"
#include "../core/input_processor.hpp"

namespace Window
{
GLWindow::GLWindow(std::string title) :
    _inputProcessor(std::make_shared<InputProcessor>()),
    _title(title)
{

}

GLWindow::~GLWindow()
{

}

void GLWindow::processFramebufferResize(int width, int height)
{
    _inputProcessor->processFramebufferResize(shared_from_this(), width, height);
}

void GLWindow::processKeyboard(Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    _inputProcessor->processKeyboard(shared_from_this(), key, scancode, action, mods);
}

void GLWindow::processMouseButton(Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    _inputProcessor->processMouseButton(shared_from_this(), button, action, mods);
}

void GLWindow::processMouseCursor(double xpos, double ypos)
{
    _inputProcessor->processMouseCursor(shared_from_this(), xpos, ypos);
}

void GLWindow::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessor = inputProcessor;
}

void GLWindow::detachInputProcessor()
{
    _inputProcessor = std::make_shared<InputProcessor>();
}

std::string GLWindow::getTitle()
{
    return _title;
}
}