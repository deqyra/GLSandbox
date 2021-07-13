#ifndef RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_MANAGER_HPP
#define RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_MANAGER_HPP

#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include <cpptools/oo/interfaces/event_receiver.hpp>

#include "gl_context_event.hpp"

namespace Renderboi
{

namespace Window
{

class GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

/// @brief Implementation of a class able to receive render events which were
/// queued to a thread running on a GL context.
///
/// @tparam E Type of the events to be received. 
class GLContextEventManager : public cpptools::EventReceiver<GLContextEvent>
{
public:
    /// @param window Pointer to the window linked to the sandbox whose events
    ///to manage.
    GLContextEventManager(GLWindowPtr window);

    /// @brief Process all events that were registered, and clear the
    /// queue. May be called from the rendering thread only, as this may in 
    /// turn issue gl* calls.
    virtual void processPendingEvents();

    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from EventReceiver ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Register an event to the entity.
    ///
    /// @param event Literal describing the event to queue.
    virtual void queueEvent(const GLContextEvent& event) override;

protected:
    /// @brief Process a single event in the queue.
    ///
    /// @param event Literal describing the event to process.
    void _processEvent(const GLContextEvent& event);

    /// @brief Pointer to the window whose events to manage.
    GLWindowPtr _window;

    /// @brief Mutex for accessing the event queue.
    std::mutex _eventMutex;

    /// @brief Queue for events.
    std::queue<GLContextEvent> _eventQueue;
};

using GLContextEventManagerPtr = std::shared_ptr<GLContextEventManager>;

}// namespace Window

}// namespace Renderboi


#endif//RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_MANAGER_HPP