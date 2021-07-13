#ifndef RENDERBOI__WINDOW__GL_CONTEXT_CLIENT_HPP
#define RENDERBOI__WINDOW__GL_CONTEXT_CLIENT_HPP

#include <memory>

#include <cpptools/oo/interfaces/event_receiver.hpp>

#include "event/gl_context_event.hpp"
#include "event/gl_context_event_manager.hpp"

namespace Renderboi
{

namespace Window
{

class GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

class GLContextClient
{
public:
    using GLContextEventReceiver = cpptools::EventReceiver<GLContextEvent>;
    using GLContextEventReceiverPtr = cpptools::EventReceiverPtr<GLContextEvent>;

    /// @param window Pointer to the window in charge of painting the context.
    GLContextClient(GLWindowPtr window);

    /// @param window Pointer to the window in charge of painting the context.
    /// @param eventManager Pointer to entity which will process the context events.
    GLContextClient(GLWindowPtr window, GLContextEventManagerPtr eventManager);

    /// @brief Pointer to the render event queue manager of the sandbox.
    GLContextEventManagerPtr eventManager;

protected:
    /// @brief Pointer to the window painting the context.
    GLWindowPtr _window;
};

using GLContextClientPtr = std::shared_ptr<GLContextClient>;

} // namespace Window

} // namespace Renderboi

#endif//RENDERBOI__WINDOW__GL_CONTEXT_CLIENT_HPP