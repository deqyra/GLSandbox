#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP

#include "../window_factory.hpp"
#include "../window_backend.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

template<>
class WindowFactory<WindowBackend::GLFW3>
{
    public:
        using ErrorCallbackSignature = GLFWerrorfun;

        static int initializeBackend();
        static void terminateBackend();
        static void setErrorCallback(const void* callback);
        static GLWindowPtr makeWindow(std::string title, int width, int height, int glVersionMajor, int glVersionMinor, Window::OpenGLProfile glProfile, bool debug);
};

#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_FACTORY_HPP