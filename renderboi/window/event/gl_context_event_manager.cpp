#include "gl_context_event_manager.hpp"

#include <glad/gl.h>

#include "../gl_window.hpp"

namespace Renderboi
{

namespace Window
{

GLContextEventManager::GLContextEventManager(GLWindowPtr window) :
    _window(window),
    _eventMutex(),
    _eventQueue()
{

}

void GLContextEventManager::processPendingEvents()
{
    _eventMutex.lock();
    bool empty = _eventQueue.empty();
    _eventMutex.unlock();

    if (empty)
    {
        return;
    }

    // Process all awaiting events
    while (!_eventQueue.empty())
    {
        _eventMutex.lock();
        GLContextEvent e = _eventQueue.front();
        _eventMutex.unlock();

        _processEvent(e);

        _eventMutex.lock();
        _eventQueue.pop();
        _eventMutex.unlock();
    }
}

void GLContextEventManager::queueEvent(const GLContextEvent& event)
{
    _eventMutex.lock();
    _eventQueue.push(event);
    _eventMutex.unlock();
}

void GLContextEventManager::_processEvent(const GLContextEvent& event)
{
    switch (event)
    {
        case GLContextEvent::FitFramebufferToWindow:
            int w, h;
            _window->getFramebufferSize(w, h);
            glViewport(0, 0, w, h);
            break;

        default:
            std::string s = "GLContextEventManager: cannot process unknown event"
            + std::to_string(static_cast<int>(event)) + "."; 
            throw std::runtime_error(s.c_str());
    }
}

}// namespace Window

}// namespace Renderboi
