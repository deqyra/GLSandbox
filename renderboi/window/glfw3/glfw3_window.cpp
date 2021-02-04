#include "glfw3_window.hpp"

#include <iostream>
#include <string>

#include "../enums.hpp"
#include "glfw3_adapter.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "glfw3_gamepad_manager.hpp"

namespace Renderboi
{

GLFW3Window::GLFW3Window(GLFWwindow* window, std::string title) :
    GLWindow(title),
    _w(window)
{
    _gamepadManager = std::static_pointer_cast<GamepadManager>(std::make_shared<GLFW3GamepadManager>());
    _gamepadManager->pollPresentGamepads();
    glfwGetFramebufferSize(_w, (int*)&_width, (int*)&_height);
}

GLFW3Window::~GLFW3Window()
{
    glfwDestroyWindow(_w);
}

bool GLFW3Window::shouldClose() const
{
    return glfwWindowShouldClose(_w);
}

void GLFW3Window::setShouldClose(bool value)
{
    glfwSetWindowShouldClose(_w, value);
}

void GLFW3Window::swapBuffers()
{
    glfwSwapBuffers(_w);
}

void GLFW3Window::pollEvents() const
{
    glfwPollEvents();
}

void GLFW3Window::setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value)
{
    unsigned int glfw3Target = Window::GLFW3Adapter::getValue(target);
    unsigned int glfw3Value = Window::GLFW3Adapter::getValue(value);
    glfwSetInputMode(_w, glfw3Target, glfw3Value);
}

void GLFW3Window::setTitle(std::string title)
{
    _title = title;
    glfwSetWindowTitle(_w, _title.c_str());
}

void GLFW3Window::getCursorPos(double& x, double& y) const
{
    glfwGetCursorPos(_w, &x, &y);
}

void GLFW3Window::makeContextCurrent()
{
    glfwMakeContextCurrent(_w);
}

void GLFW3Window::releaseContext()
{
    glfwMakeContextCurrent(nullptr);
}

float GLFW3Window::getAspectRatio() const
{
    return (float)_width / (float)_height;
}

}//namespace Renderboi
