#ifndef RENDERBOI__TOOLBOX__INTERFACES__ACTION_EVENT_RECEIVER_HPP
#define RENDERBOI__TOOLBOX__INTERFACES__ACTION_EVENT_RECEIVER_HPP

#include <memory>

#include <renderboi/window/gl_window.hpp>

namespace Renderboi
{

/// @brief Interface for an object able to receive and process events related to
/// an action of a certain type.
///
/// @tparam T Type of the action to be handled by the implementing class.
template<typename T>
class ActionEventReceiver
{
public:
    /// @brief Start the processing for an action.
    ///
    /// @param window Pointer to the window on which the action should be
    /// started.
    /// @param action Object describing the action to start processing.
    virtual void triggerAction(const GLWindowPtr window, const T& action) = 0;

    /// @brief Stop the processing for an action.
    ///
    /// @param window Pointer to the window on which the action should be
    /// stopped.
    /// @param action Object describing the action to stop processing.
    virtual void stopAction(const GLWindowPtr window, const T& action) = 0;
};

template<typename T>
using ActionEventReceiverPtr = std::shared_ptr<ActionEventReceiver<T>>;

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__INTERFACES__ACTION_EVENT_RECEIVER_HPP