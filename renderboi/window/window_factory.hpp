#ifndef RENDERBOI__WINDOW__WINDOW_FACTORY_HPP
#define RENDERBOI__WINDOW__WINDOW_FACTORY_HPP

#include <string>

#include <glad/gl.h>

#include "gl_window.hpp"
#include "window_backend.hpp"

/// @brief Abstraction of an OS-instantiated window.
///
/// @tparam W Literal describing the window backend to use to create the window.
template<WindowBackend W>
class WindowFactory
{
    public:
        using ErrorCallbackSignature = void(void);

        /// @brief Initialize the window backend in use.
        static int initializeBackend() = delete;

        /// @brief Terminate the window backend in use.
        static void terminateBackend() = delete;

        /// @brief Set the error callback for created windows.
        ///
        /// @param callback Function pointer to the callback to use for error
        /// reporting.
        static void setErrorCallback(const void* callback) = delete;

        /// @brief Create an OpenGL context within a window, bind callbacks and 
        /// initialize OpenGL function pointers.
        ///
        /// @param title Title of the window.
        /// @param width Width of the window.
        /// @param height Height of the window.
        /// @param glVersionMajor Version major of the OpenGL specification to use.
        /// @param glVersionMinor Version minor of the OpenGL specification to use.
        /// @param glProfile Literal describing the OpenGL profile to use.
        /// @param bool Whether or not to print out the debugging output.
        ///
        /// @return A pointer to an instantiated GLWindow.
        ///
        /// @exception If the window could not be created, or if the OpenGL function
        /// pointers could not be loaded, the function will throw a 
        /// std::runtime_error. If a debug capable context was requested but the 
        /// environment does not support GL_ARB_debug_output, the function will 
        /// throw a std::runtime_error.
        static GLWindowPtr makeWindow(std::string title, int width, int height, int glVersionMajor, int glVersionMinor, Window::OpenGLProfile glProfile, bool debug) = delete;
};

/// @brief Callback given to OpenGL, which will print out error messages from
/// debug capable contexts.
void APIENTRY glDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

#endif//RENDERBOI__WINDOW__WINDOW_FACTORY_HPP