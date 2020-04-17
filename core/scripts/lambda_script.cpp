#include "lambda_script.hpp"

LambdaScript::LambdaScript() :
    updateCallback([](float){}),
    framebufferResizeCallback([](GLFWwindow*, int, int){}),
    keyboardCallback([](GLFWwindow*, int, int, int, int){}),
    mouseButtonCallback([](GLFWwindow*, int, int, int){}),
    mouseCursorCallback([](GLFWwindow*, double, double){})
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

LambdaScript* LambdaScript::clone()
{
    return new LambdaScript();
}
