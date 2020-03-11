#include "basic_input_manager.hpp"

#include <iostream>
#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

BasicInputManager::BasicInputManager()
{

}

void BasicInputManager::processFramebufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void BasicInputManager::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      // Normal draw mode
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Wireframe draw mode
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);     // Vertices only draw mode
    }

    // cout << "Key event: " << glfwKeyName(key) << " " << glfwActionName(action) << endl;
}

void BasicInputManager::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    std::cout << "x: " << xpos << "; y: " << ypos << "; event: mouse " << glfwButtonName(button) << " " << glfwActionName(action) << std::endl;
}

void BasicInputManager::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{

}

std::string glfwActionName(int action)
{
    switch (action)
    {
        case GLFW_PRESS:
            return "pressed";
        case GLFW_REPEAT:
            return "held";
        case GLFW_RELEASE:
            return "released";
        default:
            return "???";
    }
}

std::string glfwKeyName(int key)
{
    if (key >= 0 && key <= 255)
    {
        return std::to_string((char)(key));
    }
    else
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                return "Escape";
            case GLFW_KEY_SPACE:
                return "Space";
            case GLFW_KEY_ENTER:
                return "Enter";
            case GLFW_KEY_LEFT:
                return "left arrow";
            case GLFW_KEY_RIGHT:
                return "right arrow";
            case GLFW_KEY_UP:
                return "up arrow";
            case GLFW_KEY_DOWN:
                return "down arrow";
            default:
                return "???";
        }
    }
}

std::string glfwButtonName(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            return "left button";
        case GLFW_MOUSE_BUTTON_RIGHT:
            return "right button";
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return "middle button";
        default:
            return std::string("button ") + std::to_string(button + 1);
    }
}
