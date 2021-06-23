#ifndef RENDERBOI__WINDOW__WINDOW_FACTORY_HPP
#define RENDERBOI__WINDOW__WINDOW_FACTORY_HPP

#include <string>

#include <glad/gl.h>

#include "gl_window.hpp"
#include "monitor.hpp"
#include "window_backend.hpp"
#include "window_creation_parameters.hpp"

namespace Renderboi::Window
{

/// @brief Collection of functions to instantiate a window on the OS-level. 
/// These must be called only from the main thread.
///
/// @tparam W Literal describing the window backend to use to create the window.
template<WindowBackend W>
class WindowFactory
{
public:
    using ErrorCallbackSignature = void(void);

    /// @brief Initialize the window backend in use. To be called before any
    /// other function is used.
    ///
    /// @param callback Optional pointer to the callback to use for error
    /// reporting by the window backend.
    static int InitializeBackend() = delete;

    /// @brief Terminate the window backend in use. Call after being done using
    /// the window backend. Call InitializeBackend() before using any other
    /// function again.
    static void TerminateBackend() = delete;

    /// @brief Set the error callback for the window backend.
    ///
    /// @param callback Function pointer to the callback to use for error
    /// reporting.
    static void SetErrorCallback(const void* callback) = delete;

    /// @brief Get a pointer to the primary monitor of the system.
    static MonitorPtr GetPrimaryMonitor() = delete;

    /// @brief Get an array filled with pointers to the different monitors of
    /// the system.
    static std::vector<MonitorPtr> GetMonitors() = delete;

    /// @brief Set the callback for monitor configuration changes.
    ///
    /// @param callback Function pointer to the callback to use for monitor
    /// configuration change.
    static void SetMonitorCallback(const void* callback) = delete;

    /// @brief Get the desktop video mode of the monitor. Implementation may not
    /// always return exact results under particular circumstances.
    ///
    /// @param monitor Pointer to the monitor whose native video mode to retrieve.
    static Monitor::VideoMode GetMonitorNativeVideoMode(const MonitorPtr monitor) = delete;

    /// @brief Create a window on the system, initialize a context within it,
    /// bind callbacks, and get a pointer to it.
    ///
    /// @param params Parameters for the window creation.
    ///
    /// @return A pointer to an instantiated GLWindow.
    ///
    /// @exception If the window could not be created, or if the OpenGL function
    /// pointers could not be loaded, the function will throw a 
    /// std::runtime_error. If a debug capable context was requested but the 
    /// environment does not support GL_ARB_debug_output, the function will 
    /// throw a std::runtime_error.
    static GLWindowPtr MakeWindow(const WindowCreationParameters& params) = delete;

    /// @brief Unbind a window from the system and deallocate the resources 
    /// it uses.
    ///
    /// @param window Pointer to the window to deallocate.
    static void DestroyWindow(GLWindowPtr window) = delete;
};

}//namespace Renderboi::Window

#endif//RENDERBOI__WINDOW__WINDOW_FACTORY_HPP