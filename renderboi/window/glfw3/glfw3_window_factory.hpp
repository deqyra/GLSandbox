#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP

#include "../window_factory.hpp"
#include "../window_backend.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

namespace Renderboi
{

/// @brief GLFW3 specialization of the window factory.
template<>
class WindowFactory<WindowBackend::GLFW3>
{
    public:
        using ErrorCallbackSignature = GLFWerrorfun;

        static int InitializeBackend();

        static void TerminateBackend();

        static void SetErrorCallback(const void* callback);

        static GLWindowPtr MakeWindow(
            std::string title,
            int width,
            int height,
            int glVersionMajor,
            int glVersionMinor,
            Window::OpenGLProfile glProfile,
            bool debug
        );
};

}//namespace Renderboi

#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP