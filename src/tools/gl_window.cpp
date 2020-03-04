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

void GLWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    _inputProcessor->framebufferResizeCallback(window, width, height);
}

void GLWindow::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    _inputProcessor->keyboardCallback(window, key, scancode, action, mods);
}

void GLWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    _inputProcessor->mouseButtonCallback(window, button, action, mods);
}

void GLWindow::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    _inputProcessor->mouseCursorCallback(window, xpos, ypos);
}

void GLWindow::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessor = inputProcessor;
}

void GLWindow::removeInputProcessor()
{
    _inputProcessor = std::make_shared<InputProcessor>();
}