#include "gl_window.hpp"

#include <iostream>
#include <string>

#include "glfw3_adapter.hpp"
#include "../core/input_processor.hpp"

GLWindow::GLWindow(GLFWwindow* window, std::string title) :
    _w(window),
    _inputProcessor(std::make_shared<InputProcessor>()),
    _title(title)
{
    glfwGetFramebufferSize(_w, &_width, &_height);
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(_w);
}

GLFWwindow* GLWindow::getWindow()
{
    return _w;
}

void GLWindow::processFramebufferResize(int width, int height)
{
    _inputProcessor->processFramebufferResize(shared_from_this(), width, height);
    _width = width;
    _height = height;
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

bool GLWindow::shouldClose()
{
    return glfwWindowShouldClose(_w);
}

void GLWindow::setShouldClose(bool value)
{
    glfwSetWindowShouldClose(_w, value);
}

void GLWindow::swapBuffers()
{
    glfwSwapBuffers(_w);
}

void GLWindow::pollEvents()
{
    glfwPollEvents();
}

void GLWindow::setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value)
{
    unsigned int glfw3Target = Window::GLFW3Adapter::getValue(target);
    unsigned int glfw3Value = Window::GLFW3Adapter::getValue(value);
    glfwSetInputMode(_w, glfw3Target, glfw3Value);
}

std::string GLWindow::getTitle()
{
    return _title;
}

void GLWindow::setTitle(std::string title)
{
    _title = title;
    glfwSetWindowTitle(_w, _title.c_str());
}

float GLWindow::getAspectRatio()
{
    return (float)_width / (float)_height;
}

void GLWindow::getCursorPos(double* x, double* y)
{
    glfwGetCursorPos(_w, x, y);
}
