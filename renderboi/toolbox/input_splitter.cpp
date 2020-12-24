#include "input_splitter.hpp"

#include <stdexcept>

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

void InputSplitter::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processFramebufferResize(window, width, height);
    }
}

void InputSplitter::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processKeyboard(window, key, scancode, action, mods);
    }
}

void InputSplitter::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processMouseButton(window, button, action, mods);
    }
}

void InputSplitter::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{
    for (auto it = _subscribers.begin(); it != _subscribers.end(); it++)
    {
        it->second->processMouseCursor(window, xpos, ypos);
    }
}
