#include "input_splitter.hpp"

#include <stdexcept>

namespace Renderboi
{

InputSplitter::InputSplitter() :
    _subscriberRollingCount(0),
    _subscribers()
{

}

unsigned int InputSplitter::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    if (!inputProcessor)
    {
        throw std::runtime_error("InputSplitter: cannot register null input processor pointer.");
    }

    _subscribers[_subscriberRollingCount] = inputProcessor;
    return _subscriberRollingCount++;
}

void InputSplitter::detachInputProcessor(unsigned int subscriptionId)
{
    _subscribers.erase(subscriptionId);
}

void InputSplitter::detachAllInputProcessors()
{
    _subscribers.clear();
}

unsigned int InputSplitter::registerGamepadInputProcessor(GamepadInputProcessorPtr inputProcessor)
{
    if (!inputProcessor)
    {
        throw std::runtime_error("InputSplitter: cannot register null input processor pointer.");
    }

    _gamepadSubscribers[_subscriberRollingCount] = inputProcessor;
    return _subscriberRollingCount++;
}

void InputSplitter::detachGamepadInputProcessor(unsigned int subscriptionId)
{
    _gamepadSubscribers.erase(subscriptionId);
}

void InputSplitter::detachAllIGamepadnputProcessors()
{
    _gamepadSubscribers.clear();
}

void InputSplitter::processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height)
{
    for (const auto& [_, inputProc] : _subscribers)
    {
        inputProc->processFramebufferResize(window, width, height);
    }
}

void InputSplitter::processKeyboard(
    const GLWindowPtr window, 
    const Key key, 
    const int scancode, 
    const Action action, 
    const int mods
)
{
    for (const auto [_, inputProc] : _subscribers)
    {
        inputProc->processKeyboard(window, key, scancode, action, mods);
    }
}

void InputSplitter::processMouseButton(
    const GLWindowPtr window, 
    const MButton button, 
    const Action action, 
    const int mods
)
{
    for (const auto [_, inputProc] : _subscribers)
    {
        inputProc->processMouseButton(window, button, action, mods);
    }
}

void InputSplitter::processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos)
{
    for (const auto [_, inputProc] : _subscribers)
    {
        inputProc->processMouseCursor(window, xpos, ypos);
    }
}

void InputSplitter::processConnected(const GamepadPtr gamepad)
{
    for (const auto [_, inputProc] : _gamepadSubscribers)
    {
        inputProc->processConnected(gamepad);
    }
}

void InputSplitter::processDisconnected(const GamepadPtr gamepad)
{
    for (const auto [_, inputProc] : _gamepadSubscribers)
    {
        inputProc->processDisconnected(gamepad);
    }
}

void InputSplitter::processButton(const GamepadPtr gamepad, const GButton button, const Action action)
{
    for (const auto [_, inputProc] : _gamepadSubscribers)
    {
        inputProc->processButton(gamepad, button, action);
    }
}

void InputSplitter::processAxis(const GamepadPtr gamepad, const Axis axis, const float value)
{
    for (const auto [_, inputProc] : _gamepadSubscribers)
    {
        inputProc->processAxis(gamepad, axis, value);
    }
}

}//namespace Renderboi
