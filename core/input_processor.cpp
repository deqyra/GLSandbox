#include "input_processor.hpp"
#include "../include/GLFW/glfw3.h"

// Initialize static stuff
unsigned int InputProcessor::_count = 0;

InputProcessor::InputProcessor() :
    id(_count++)                    // Generate ID from instance count
{

}

// Default callbacks do nothing

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
