#ifndef RENDERBOI__WINDOW__EVENT__GL_WINDOW_CRITICAL_EVENT_HPP
#define RENDERBOI__WINDOW__EVENT__GL_WINDOW_CRITICAL_EVENT_HPP

namespace Renderboi
{

namespace Window
{

/// @brief Events to be registered to a GLWindow when they are detected,
/// to be handled separately as they require special care.
enum class GLWindowCriticalEvent
{
    GoFullscreen,
    ExitFullscreen
};

}

}

#endif//RENDERBOI__WINDOW__EVENT__GL_WINDOW_CRITICAL_EVENT_HPP