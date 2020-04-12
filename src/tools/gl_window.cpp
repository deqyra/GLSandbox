#include "gl_window.hpp"

#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

#include "input_processor.hpp"

GLWindow::GLWindow(GLFWwindow* window) :
    _w(window),
    _inputProcessor(std::make_shared<InputProcessor>())
{

}

void GLWindow::processFramebufferResize(GLFWwindow* window, int width, int height)
{
    _inputProcessor->processFramebufferResize(window, width, height);
}

void GLWindow::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    _inputProcessor->processKeyboard(window, key, scancode, action, mods);
}

void GLWindow::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    _inputProcessor->processMouseButton(window, button, action, mods);
}

void GLWindow::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{
    _inputProcessor->processMouseCursor(window, xpos, ypos);
}

void GLWindow::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessor = inputProcessor;
}

void GLWindow::detachInputProcessor()
{
    _inputProcessor = std::make_shared<InputProcessor>();
}