#ifndef RENDERBOI__WINDOW__EVENT__CRITICAL_EVENT_MANAGER_HPP
#define RENDERBOI__WINDOW__EVENT__CRITICAL_EVENT_MANAGER_HPP

#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include <cpptools/thread/interfaces/interruptible.hpp>

#include "gl_window_critical_event.hpp"
#include "../interfaces/critical_event_receiver.hpp"

namespace Renderboi
{

namespace Window
{

class GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

/// @brief Implementation of a class able to receive critical events which were
/// queued to a window. Such a class is to be used in context with windows.
///
/// @tparam E Type of the critical events to be received. 
class CriticalEventManager : public CriticalEventReceiver<GLWindowCriticalEvent>
{
private:
    friend GLWindow;
    using InterruptiblePtr = cpptools::InterruptiblePtr;

    /// @param window Naked pointer to the window whose critical events to manage.
    /// It's okay to go with a naked pointer because a CriticalEventManager  
    /// instance necessarily tied to a single GLWindow which itself manages the 
    /// lifetime of that instance.
    CriticalEventManager(GLWindow* window);

public:
    /// @brief Process all criticals events that were registered, and clear the
    /// queue. In order to safely process critical events, the registered 
    /// critical processes will be paused. May be called from the main thread only.
    virtual void processPendingCriticalEvents();

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from CriticalEventReceiver ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Registers an interruptible process which uses critical resources
    /// that are shared with the window. Having these registered allows to pause 
    /// them before processing critical events as they come up, thereby avoiding
    /// execution of code in an invalid context. May be called from the main 
    /// thread only.
    ///
    /// @note Taking care of registering ALL such critical processes is left to 
    /// the user. If not all critical processes are registered but critical 
    /// events are still queued for processing, the behaviour becomes undefined 
    /// and may result in crashes.
    ///
    /// @param process Pointer to the interruptible process.
    ///
    /// @return The registration ID for the registered process.
    ///
    /// @exception If the provided pointer is null, the function will throw an
    /// std::runtime_error.
    virtual unsigned int registerCriticalProcess(InterruptiblePtr process) override;

    /// @brief Remove a critical process which was previously registered to the
    /// window. May be called from the main thread only.
    ///
    /// @param registrationId ID which was returned upon calling 
    /// registerCriticalProcess in the first place.
    virtual void detachCriticalProcess(const unsigned int registrationId) override;

    /// @brief Register a critical event to the window, which must then be 
    /// processed with processCriticalEventQueue(). May be called from any thread.
    ///
    /// @param event Literal describing the event to queue.
    virtual void queueEvent(const GLWindowCriticalEvent& event) override;

private:
    /// @brief Process a single critical event in the queue.
    ///
    /// @param event Literal describing the event to process.
    void _processCriticalEvent(const GLWindowCriticalEvent& event);

    /// @brief Pointer to the window whose critical events to manage.
    GLWindow* _window;

    /// @brief Mutex for accessing the critical event queue.
    std::mutex _criticalEventMutex;

    /// @brief Queue for critical events.
    std::queue<GLWindowCriticalEvent> _criticalEventQueue;

    /// @brief List of processes to interrupt before processing critical events.
    std::map<unsigned int, InterruptiblePtr> _criticalProcesses;

    /// @brief Rolling process count, used as an ID system to manage registration.
    unsigned int _processCount;
};

using CriticalEventManagerPtr = std::shared_ptr<CriticalEventManager>;

}// namespace Window


}// namespace Renderboi


#endif//RENDERBOI__WINDOW__EVENT__CRITICAL_EVENT_MANAGER_HPP