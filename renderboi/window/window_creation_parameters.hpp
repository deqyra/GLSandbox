#ifndef RENDERBOI__WINDOW__WINDOW_CREATION_PARAMETERS_HPP
#define RENDERBOI__WINDOW__WINDOW_CREATION_PARAMETERS_HPP

#include <string>

#include "enums.hpp"
#include "gl_window.hpp"
#include "monitor.hpp"

namespace Renderboi::Window
{

struct WindowCreationParameters
{
    /// @brief Title of the window.
    std::string title;

    /// @brief Width of the window in screen coordinates.
    int width;

    /// @brief Height of the window in screen coordinates.
    int height;

    /// @brief Whether or not the window is resizable.
    bool resizable;

    /// @brief Version major of the OpenGL context to create. Will be ignored if
    /// requesting a context shared from another window.
    int glVersionMajor;

    /// @brief Version major of the OpenGL context to create. Will be ignored if
    /// requesting a context shared from another window.
    int glVersionMinor;

    /// @brief Profile of the OpenGL context to use. May be ignored on certain
    /// platforms. Will be ignored if requesting a context shared from another 
    /// window.
    Window::OpenGLProfile glProfile;

    /// @brief Pointer to the window whose context to share. Pass nullptr to 
    /// instantiate a new context.
    GLWindowPtr shareContext;

    /// @brief Monitor on which the window should go fullscreen. Pass nullptr to
    /// stay in windowed mode.
    MonitorPtr monitor;

    /// @brief If the window is requested to go fullscreen on a monitor and this
    /// is enabled, the window will fit exactly the current video mode of the 
    /// monitor it is on.
    bool borderlessFullscreen;

    /// @brief Whether or not the window should be automatically minimized when
    /// losing input focus while in full screen mode. Disabling this is useful
    /// when using several displays in fullscreen.
    bool autoMinimize;

    /// @brief Whether or not the window should be decorated with system widgets
    /// like a border, a window title bar with buttons and such.
    bool decorated;

    /// @brief Whether the window should have a transparent framebuffer, which
    /// will let the desktop compositor blend the window contents with its
    /// background.
    bool transparentFramebuffer;

    /// @brief Whether or not the window should be visible.
    bool visible;

    /// @brief Whether or not the window should be maximized when created.
    bool maximized;

    /// @brief Whether or not the window should stay above other windows.
    bool alwaysOnTop;

    /// @brief Whether or not the window should get the input focus when created.
    bool focused;

    /// @brief Whether or not the window should get the input focus when shown.
    bool focusOnShow;

    /// @brief Whether or not to request a debug context.
    bool debug;
};

const WindowCreationParameters DefaultWindowCreationParameters = {
    .title                      = "New window",
    .width                      = 640,
    .height                     = 480,
    .resizable                  = true,
    .glVersionMajor             = 4,
    .glVersionMinor             = 2,
    .glProfile                  = Window::OpenGLProfile::Core,
    .shareContext               = nullptr,
    .monitor                    = nullptr,
    .autoMinimize               = false,
    .decorated                  = true,
    .transparentFramebuffer     = false,
    .visible                    = true,
    .maximized                  = false,
    .alwaysOnTop                = false,
    .focused                    = true,
    .focusOnShow                = true,
    .debug                      = false
};

}//namespace Renderboi::Window

#endif//RENDERBOI__WINDOW__WINDOW_CREATION_PARAMETERS_HPP