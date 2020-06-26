#include "glfw3_window.hpp"

#include <iostream>
#include <string>

#include "glfw3_adapter.hpp"
#include "../../core/input_processor.hpp"

namespace Window
{
GLFW3Window::GLFW3Window(GLFWwindow* window, std::string title) :
    GLWindow(title),
    _w(window)
{

}

GLFW3Window::~GLFW3Window()
{
    glfwDestroyWindow(_w);
}

bool GLFW3Window::shouldClose()
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

void GLFW3Window::pollEvents()
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

void GLFW3Window::getCursorPos(double* x, double* y)
{
    glfwGetCursorPos(_w, x, y);
}
}
