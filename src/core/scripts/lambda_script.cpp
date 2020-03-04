#include "lambda_script.hpp"

LambdaScript::LambdaScript()
{

}

void LambdaScript::update(float timeElapsed)
{
    updateCallback(timeElapsed);
}

void LambdaScript::processFramebufferResize(GLFWwindow* window, int width, int height)
{
    processFramebufferResize(window, width, height);
}

void LambdaScript::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyboardCallback(window, key, scancode, action, mods);
}

void LambdaScript::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    mouseButtonCallback(window, button, action, mods);
}

void LambdaScript::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{
    mouseCursorCallback(window, xpos, ypos);
}
