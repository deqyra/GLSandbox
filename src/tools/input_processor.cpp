/**
    GLTest, input_processor.cpp
    Purpose: Implementation of class InputProcessor. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 14/09/2019
 */
#include "input_processor.hpp"
#include "../include/GLFW/glfw3.h"

unsigned int InputProcessor::_count = 0;

InputProcessor::InputProcessor() :
    id(_count++)
{

}

void InputProcessor::processFramebufferResize(GLFWwindow* window, int width, int height)
{

}

void InputProcessor::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void InputProcessor::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{

}

void InputProcessor::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{

}
