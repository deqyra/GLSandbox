#include "gl_context_client.hpp"

namespace Renderboi
{

namespace Window
{

GLContextClient::GLContextClient(GLWindowPtr window) :
    _window(window),
    eventManager(std::make_shared<GLContextEventManager>(window))
{

}

GLContextClient::GLContextClient(GLWindowPtr window, GLContextEventManagerPtr eventManager) :
    _window(window),
    eventManager(eventManager)
{

}

} // namespace Window

} // namespace Renderboi
