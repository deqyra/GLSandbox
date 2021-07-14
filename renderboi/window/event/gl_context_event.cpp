#include "gl_context_event.hpp"

#include <unordered_map>

namespace Renderboi
{

using Window::GLContextEvent;

std::string to_string(const GLContextEvent& event)
{
    static bool runOnce = false;
    static std::unordered_map<GLContextEvent, std::string> enumNames;

    if (!runOnce)
    {
        enumNames[GLContextEvent::FitFramebufferToWindow] = "FitFramebufferToWindow";
        enumNames[GLContextEvent::PolygonModeFill]        = "PolygonModeFill";
        enumNames[GLContextEvent::PolygonModeLine]        = "PolygonModeLine";
        enumNames[GLContextEvent::PolygonModePoint]       = "PolygonModePoint";

        runOnce = true;
    }

    auto it = enumNames.find(event);
    if (it != enumNames.end()) return it->second;

    return "Unknown";
}

} // namespace Renderboi
