/**
    GLTest, GLWindow.cpp
    Purpose: Implementation of class GLWindow. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 19/06/2019
 */
#include "gl_window.hpp"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

#include "gl_utils.hpp"
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

void GLWindow::removeInputProcessor()
{
    _inputProcessor = std::make_shared<InputProcessor>();
}