#ifndef RENDERBOI__TOOLBOX__SCRIPTS__BASIC_INPUT_MANAGER_HPP
#define RENDERBOI__TOOLBOX__SCRIPTS__BASIC_INPUT_MANAGER_HPP

#include <renderboi/window/input_processor.hpp>

#include "../controls/action_event_receiver.hpp"

enum class BasicWindowManagerAction
{
    Terminate,
    PolygonFill,
    PolygonLine,
    PolygonPoint
};

/// @brief Provides event callbacks associated with basic window management.
class BasicWindowManager : public ActionEventReceiver<BasicWindowManagerAction>, public InputProcessor
{
    private:
        BasicWindowManager(const BasicWindowManager& other) = delete;
        BasicWindowManager& operator=(const BasicWindowManager& other) = delete;

    public:
        BasicWindowManager();

        /////////////////////////////////////////////////////////////////////////////
        ///                                                                       ///
        /// Methods overridden from ActionEventReceiver<BasicWindowManagerAction> ///
        ///                                                                       ///
        /////////////////////////////////////////////////////////////////////////////

        /// @brief Start the processing for an action.
        ///
        /// @param action Object describing the action to start processing.
        void triggerAction(GLWindowPtr window, const BasicWindowManagerAction& action);

        /// @brief Stop the processing for an action.
        ///
        /// @param action Object describing the action to stop processing.
        void stopAction(GLWindowPtr window, const BasicWindowManagerAction& action);

        //////////////////////////////////////////////
        ///                                        ///
        /// Methods overridden from InputProcessor ///
        ///                                        ///
        //////////////////////////////////////////////

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        void processFramebufferResize(GLWindowPtr window, int width, int height);

};

#endif//RENDERBOI__TOOLBOX__SCRIPTS__BASIC_INPUT_MANAGER_HPP