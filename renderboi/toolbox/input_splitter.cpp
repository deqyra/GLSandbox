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
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processFramebufferResize(window, width, height);
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
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processKeyboard(window, key, scancode, action, mods);
    }
}

void InputSplitter::processMouseButton(
    const GLWindowPtr window, 
    const MButton button, 
    const Action action, 
    const int mods
)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processMouseButton(window, button, action, mods);
    }
}

void InputSplitter::processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processMouseCursor(window, xpos, ypos);
    }
}

void InputSplitter::processConnected(const GamepadPtr gamepad)
{
    for (auto it = _gamepadSubscribers.begin(); it != _gamepadSubscribers.end(); it++)
    {
        it->second->processConnected(gamepad);
    }
}

void InputSplitter::processDisconnected(const GamepadPtr gamepad)
{
    for (auto it = _gamepadSubscribers.begin(); it != _gamepadSubscribers.end(); it++)
    {
        it->second->processDisconnected(gamepad);
    }
}

void InputSplitter::processButton(const GamepadPtr gamepad, const GButton button, const Action action)
{
    for (auto it = _gamepadSubscribers.begin(); it != _gamepadSubscribers.end(); it++)
    {
        it->second->processButton(gamepad, button, action);
    }
}

void InputSplitter::processAxis(const GamepadPtr gamepad, const Axis axis, const float value)
{
    for (auto it = _gamepadSubscribers.begin(); it != _gamepadSubscribers.end(); it++)
    {
        it->second->processAxis(gamepad, axis, value);
    }
}

}//namespace Renderboi
