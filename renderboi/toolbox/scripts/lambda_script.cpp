#include "lambda_script.hpp"

LambdaScript::LambdaScript() :
    updateCallback([](float){}),
    framebufferResizeCallback([](GLWindowPtr, int, int){}),
    keyboardCallback([](GLWindowPtr, Window::Input::Key, int, Window::Input::Action, int){}),
    mouseButtonCallback([](GLWindowPtr, Window::Input::MouseButton, Window::Input::Action, int){}),
    mouseCursorCallback([](GLWindowPtr, double, double){})
{

}

void LambdaScript::update(float timeElapsed)
{
    updateCallback(timeElapsed);
}

void LambdaScript::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    processFramebufferResize(window, width, height);
}

void LambdaScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    keyboardCallback(window, key, scancode, action, mods);
}

void LambdaScript::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    mouseButtonCallback(window, button, action, mods);
}

void LambdaScript::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{
    mouseCursorCallback(window, xpos, ypos);
}

LambdaScript* LambdaScript::clone()
{
    return new LambdaScript();
}
