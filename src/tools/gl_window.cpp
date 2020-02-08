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
    _baseInputProcessor(),
    _customInputProcessor(nullptr)
{

}

void GLWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    if (_customInputProcessor)
    {
        _customInputProcessor->framebufferResizeCallback(window, width, height);
    }
    else
    {
        _baseInputProcessor.framebufferResizeCallback(window, width, height);
    }
}

void GLWindow::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (_customInputProcessor)
    {
        _customInputProcessor->keyboardCallback(window, key, scancode, action, mods);
    }
    else
    {
        _baseInputProcessor.keyboardCallback(window, key, scancode, action, mods);
    }
}

void GLWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (_customInputProcessor)
    {
        _customInputProcessor->mouseButtonCallback(window, button, action, mods);
    }
    else
    {
        _baseInputProcessor.mouseButtonCallback(window, button, action, mods);
    }
}

void GLWindow::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (_customInputProcessor)
    {
        _customInputProcessor->mouseCursorCallback(window, xpos, ypos);
    }
    else
    {
        _baseInputProcessor.mouseCursorCallback(window, xpos, ypos);
    }
}

void GLWindow::registerInputProcessor(InputProcessor* inputProcessor)
{
    _customInputProcessor = inputProcessor;
}

void GLWindow::removeInputProcessor()
{
    _customInputProcessor = nullptr;
}