#ifndef RENDERBOI__TOOLBOX__CONTROLS__ACTION_EVENT_RECEIVER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__ACTION_EVENT_RECEIVER_HPP

#include <memory>

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
        /// @param action Object describing the action to start processing.
        virtual void triggerAction(const T& action) = 0;

        /// @brief Stop the processing for an action.
        ///
        /// @param action Object describing the action to stop processing.
        virtual void stopAction(const T& action) = 0;
};

template<typename T>
using ActionEventReceiverPtr = std::shared_ptr<ActionEventReceiver<T>>;

#endif//RENDERBOI__TOOLBOX__CONTROLS__ACTION_EVENT_RECEIVER_HPP