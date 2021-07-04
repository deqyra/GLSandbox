#include "glfw3_window.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"
#include "../env_info.hpp"
#include "glfw3_adapter.hpp"
#include "glfw3_gamepad_manager.hpp"
#include "glfw3_monitor.hpp"
#include "glfw3_window_factory.hpp"

namespace Renderboi::Window
{

using GLFW3WindowFactory = WindowFactory<WindowBackend::GLFW3>;

GLFW3Window::GLFW3Window(GLFWwindow* window, std::string title) :
    GLWindow(title),
    _w(window)
{
    _gamepadManager = std::static_pointer_cast<GamepadManager>(std::make_shared<GLFW3GamepadManager>());
    _gamepadManager->pollPresentGamepads();
}

GLFW3Window::~GLFW3Window()
{
    glfwDestroyWindow(_w);
}

void GLFW3Window::setTitle(std::string title)
{
    _title = title;
    glfwSetWindowTitle(_w, _title.c_str());
}

void GLFW3Window::setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value)
{
    unsigned int glfw3Target = Window::GLFW3Adapter::getValue(target);
    unsigned int glfw3Value = Window::GLFW3Adapter::getValue(value);
    glfwSetInputMode(_w, glfw3Target, glfw3Value);
}

void GLFW3Window::hide()
{
    glfwHideWindow(_w);
}

void GLFW3Window::show()
{
    glfwShowWindow(_w);
}

bool GLFW3Window::isVisible() const
{
    return glfwGetWindowAttrib(_w, GLFW_VISIBLE);
}

void GLFW3Window::focus()
{
    glfwFocusWindow(_w);
}

bool GLFW3Window::isFocused() const
{
    return glfwGetWindowAttrib(_w, GLFW_FOCUSED);
}

void GLFW3Window::maximize()
{
    glfwMaximizeWindow(_w);
}

bool GLFW3Window::isMaximized() const
{
    return glfwGetWindowAttrib(_w, GLFW_MAXIMIZED);
}

void GLFW3Window::minimize()
{
    glfwIconifyWindow(_w);
}

bool GLFW3Window::isMinimized() const
{
    return glfwGetWindowAttrib(_w, GLFW_ICONIFIED);
}

void GLFW3Window::getSize(int& width, int& height) const
{

}

void GLFW3Window::getFramebufferSize(int& width, int& height) const
{

}

void GLFW3Window::goFullscreen(MonitorPtr monitor, bool borderless)
{
    if (!monitor) monitor = GLFW3WindowFactory::GetPrimaryMonitor();

    GLFW3MonitorPtr glfwMonitor = std::static_pointer_cast<GLFW3Monitor>(monitor);

    glfwGetWindowSize(_w, &_widthBeforeFullscreen, &_heightBeforeFullscreen);
    glfwGetWindowPos(_w, &_xPosBeforeFullscreen, &_yPosBeforeFullscreen);

    int width = _widthBeforeFullscreen;
    int height = _heightBeforeFullscreen;
    int refreshRate = GLFW_DONT_CARE;

    if (borderless)
    {
        // To be borderless fullscreen, the app must go fullscreen in the same
        // video mode as the desktop video mode of the monitor.
        //
        // - OPTION 1: It can be assumed that it will be the video mode that the
        // monitor is in when GLFW is initialized, which can then be saved 
        // somewhere and restored (there is, however, no strong guarantee that 
        // the queried video mode is the actual desktop video mode of the
        // monitor).
        // (used when GLFW3_BORDERLESS_POLICY_NATIVE is defined)
        //
        // - or, OPTION 2: It can be assumed that this video mode will simply be
        // the largest of those supported by the monitor, although this is not
        // necessarily true;
        // (used when GLFW3_BORDERLESS_POLICY_MAX_MODE is defined)
        
#if defined(GLFW3_BORDERLESS_POLICY_NATIVE)
        Monitor::VideoMode mode = GLFW3WindowFactory::GetMonitorNativeVideoMode(monitor);
        width = mode.width;
        height = mode.height;
        refreshRate = mode.refreshRate;

#else //if defined(GLFW3_BORDERLESS_POLICY_MAX_MODE)
        Monitor::VideoMode mode = glfwMonitor->getLargestVideoMode();
        width = mode.width;
        height = mode.height;
        refreshRate = mode.refreshRate;
#endif
    }

    glfwSetWindowMonitor(_w, glfwMonitor->getGlfwMonitorPointer(), 0, 0, width, height, refreshRate);
    _fullscreenMonitor = glfwMonitor->getGlfwMonitorPointer();
}

void GLFW3Window::goFullscreen(MonitorPtr monitor, int width, int height, int refreshRate)
{
    if (!monitor) monitor = GLFW3WindowFactory::GetPrimaryMonitor();
    GLFW3MonitorPtr glfwMonitor = std::static_pointer_cast<GLFW3Monitor>(monitor);

    glfwSetWindowMonitor(_w, glfwMonitor->getGlfwMonitorPointer(), 0, 0, width, height, refreshRate);
    _fullscreenMonitor = glfwMonitor->getGlfwMonitorPointer();
}

bool GLFW3Window::isFullscreen() const
{
    return _fullscreenMonitor != nullptr;
}

void GLFW3Window::exitFullscreen()
{
    glfwSetWindowMonitor(
        _w,
        nullptr,
        _xPosBeforeFullscreen,
        _yPosBeforeFullscreen,
        _widthBeforeFullscreen,
        _heightBeforeFullscreen,
        GLFW_DONT_CARE
    );
    _fullscreenMonitor = nullptr;
}

void GLFW3Window::setRefreshRate(int rate)
{
    if (!isFullscreen()) return;

    int width, height;
    glfwGetWindowSize(_w, &width, &height);
    glfwSetWindowMonitor(_w, _fullscreenMonitor, 0, 0, width, height, rate);
}

bool GLFW3Window::shouldClose() const
{
    return glfwWindowShouldClose(_w);
}

void GLFW3Window::setShouldClose(bool value)
{
    _exitSignaled = value;
    glfwSetWindowShouldClose(_w, value);
}

void GLFW3Window::swapBuffers()
{
    glfwSwapBuffers(_w);
}

void GLFW3Window::pollEvents() const
{
    glfwPollEvents();
}

float GLFW3Window::getAspectRatio() const
{
    int width, height;
    glfwGetFramebufferSize(_w, &width, &height);

    return (float)width / (float)height;
}

void GLFW3Window::getCursorPos(double& x, double& y) const
{
    glfwGetCursorPos(_w, &x, &y);
}

void GLFW3Window::makeContextCurrent()
{
    glfwMakeContextCurrent(_w);
	// Load GL pointers
	if (!gladLoadGL())
	{
        throw std::runtime_error("GLFW3Window: Failed to load GL function pointers.");
    }
}

void GLFW3Window::releaseContext()
{
    glfwMakeContextCurrent(nullptr);
}

bool GLFW3Window::extensionSupported(std::string extName)
{
    return (bool)glfwExtensionSupported(extName.c_str());
}

}//namespace Renderboi::Window
