#include "gl_window.hpp"

#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../core/input_processor.hpp"

bool GLWindow::_enumMapsPopulated = false;
std::enum_map<GLWindow::InputModeTarget, unsigned int> GLWindow::_enumTargets = std::enum_map<GLWindow::InputModeTarget, unsigned int>();
std::enum_map<GLWindow::InputModeValue, unsigned int> GLWindow::_enumValues = std::enum_map<GLWindow::InputModeValue, unsigned int>();

GLWindow::GLWindow(GLFWwindow* window, std::string title) :
    _w(window),
    _inputProcessor(std::make_shared<InputProcessor>()),
    _title(title)
{
    if (!_enumMapsPopulated) populateEnumMaps();
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(_w);
}

GLFWwindow* GLWindow::getWindow()
{
    return _w;
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

void GLWindow::setInputMode(InputModeTarget target, InputModeValue value)
{
    glfwSetInputMode(_w, _enumTargets[target], _enumValues[value]);
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

void GLWindow::populateEnumMaps()
{
    _enumTargets[InputModeTarget::Cursor] = GLFW_CURSOR;
    _enumTargets[InputModeTarget::StickyKeys] = GLFW_STICKY_KEYS;
    _enumTargets[InputModeTarget::StickyMouseButtons] = GLFW_STICKY_MOUSE_BUTTONS;
    _enumTargets[InputModeTarget::LockKeyMods] = GLFW_LOCK_KEY_MODS;
    _enumTargets[InputModeTarget::RawMouseMotion] = GLFW_RAW_MOUSE_MOTION;

    _enumValues[InputModeValue::True] = GLFW_TRUE;
    _enumValues[InputModeValue::False] = GLFW_TRUE;
    _enumValues[InputModeValue::NormalCursor] = GLFW_CURSOR_NORMAL;
    _enumValues[InputModeValue::HiddenCursor] = GLFW_CURSOR_HIDDEN;
    _enumValues[InputModeValue::DisabledCursor] = GLFW_CURSOR_DISABLED;

    _enumMapsPopulated = true;
}