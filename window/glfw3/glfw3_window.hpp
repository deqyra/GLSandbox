#ifndef WINDOW__GLFW3__GLFW3_WINDOW_HPP
#define WINDOW__GLFW3__GLFW3_WINDOW_HPP

#include "../gl_window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

namespace Window
{
// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLFW3Window : public GLWindow
{
    public:
        GLFW3Window(GLFWwindow* window, std::string title);
        ~GLFW3Window();

        // Get a pointer to the managed window
        GLFWwindow* getWindow();

        // Whether the window was flagged for closing
        bool shouldClose();
        // Set the window closing flaf
        void setShouldClose(bool value);

        // Swap GLFW buffers
        void swapBuffers();
        // Poll GLFW events
        void pollEvents();

        // Set GLFW input mode
        void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value);

        // Set title of GLFW window
        void setTitle(std::string title);

        // Get cursor position in GLFW window
        void getCursorPos(double* x, double* y);

    private:
        // The GLFW window being managed
        GLFWwindow* _w;
        // Custom input processor to be registered at any time, providing custom callbacks
        InputProcessorPtr _inputProcessor;
        // Window title
        std::string _title;
};


}//namespace Window

using GLFW3Window = Window::GLFW3Window;
using GLFW3WindowPtr = std::shared_ptr<GLFW3Window>;

#endif//WINDOW__GLFW3__GLFW3_WINDOW_HPP
