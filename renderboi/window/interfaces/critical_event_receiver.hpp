#ifndef RENDERBOI__WINDOW__CRITICAL_EVENT_RECEIVER_HPP
#define RENDERBOI__WINDOW__CRITICAL_EVENT_RECEIVER_HPP

#include <memory>

#include <cpptools/thread/interfaces/interruptible.hpp>
#include <cpptools/oo/interfaces/event_receiver.hpp>

namespace Renderboi
{

namespace Window
{

/// @brief Interface for a class able to receive critical events which were
/// queued to a window. Such a class is to be used in context with windows.
///
/// @tparam E Type of the critical events to be received. 
template<typename E>
class CriticalEventReceiver : public cpptools::EventReceiver<E>
{
private:
    using InterruptiblePtr = cpptools::InterruptiblePtr;

public:
    /// @brief Registers an interruptible process which uses critical resources
    /// that are shared with the window. Having these registered allows to pause 
    /// them before processing critical events as they come up, thereby avoiding
    /// execution of code in an invalid context.
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
    virtual unsigned int registerCriticalProcess(InterruptiblePtr process) = 0;

    /// @brief Remove a critical process which was previously registered to the
    /// window.
    ///
    /// @param registrationId ID which was returned upon calling 
    /// registerCriticalProcess in the first place.
    virtual void detachCriticalProcess(unsigned int registrationId) = 0;

    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from EventReceiver ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Register a critical event to the window.
    ///
    /// @param event Literal describing the event to queue.
    virtual void queueEvent(const E& event) = 0;
};

}// namespace Window


}// namespace Renderboi


#endif//RENDERBOI__WINDOW__CRITICAL_EVENT_RECEIVER_HPP