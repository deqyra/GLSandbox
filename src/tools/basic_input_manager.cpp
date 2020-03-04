#include "basic_input_manager.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    // cout << "Key event: " << glfwKeyName(key) << " " << glfwActionName(action) << endl;
}

void BasicInputManager::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    cout << "x: " << xpos << "; y: " << ypos << "; event: mouse " << glfwButtonName(button) << " " << glfwActionName(action) << endl;
}

void BasicInputManager::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{

}

string glfwActionName(int action)
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

string glfwKeyName(int key)
{
    if (key >= 0 && key <= 255)
    {
        stringstream sstream;
        sstream << (char)(key);
        return sstream.str();
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

string glfwButtonName(int button)
{
    switch (button)
    {
        case 0:
            return "left button";
        case 1:
            return "right button";
        case 2:
            return "middle button";
        default:
            stringstream sstream;
            sstream << "button " << button + 1;
            return sstream.str();
    }
}
