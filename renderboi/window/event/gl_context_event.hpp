#ifndef RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP
#define RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP

#include <string>

namespace Renderboi
{

namespace Window
{

enum class GLContextEvent
{
    FitFramebufferToWindow,
    PolygonModeFill,
    PolygonModeLine,
    PolygonModePoint
};

} // namespace Window

std::string to_string(const Window::GLContextEvent& event);

} // namespace Renderboi

#endif//RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP