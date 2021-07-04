#include "glfw3_window_factory.hpp"

#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <cpptools/string_tools.hpp>

#include <renderboi/utilities/gl_utilities.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include "../enums.hpp"
#include "../env_info.hpp"
#include "../gl_window.hpp"
#include "../window_backend.hpp"
#include "../window_factory.hpp"
#include "../window_creation_parameters.hpp"

#include "glfw3_adapter.hpp"
#include "glfw3_monitor.hpp"
#include "glfw3_window.hpp"
#include "glfw3_utilities.hpp"

namespace Renderboi::Window
{

using ReLoc = ResourceLocator;
using ReType = ResourceType;

std::map<unsigned int, Monitor::VideoMode>
WindowFactory<WindowBackend::GLFW3>::_nativeVideoModes = std::map<unsigned int, Monitor::VideoMode>();

std::map<unsigned int, MonitorPtr>
WindowFactory<WindowBackend::GLFW3>::_monitors = std::map<unsigned int, MonitorPtr>();

GLFWmonitorfun* WindowFactory<WindowBackend::GLFW3>::_monitorCallback = nullptr;

int WindowFactory<WindowBackend::GLFW3>::InitializeBackend()
{
    int result = glfwInit();
    if (!result) return result;

    glfwSetErrorCallback(GLFW3Utilities::globalGlfwErrorCallback);

    ///////////////////////////
    // GAMEPAD RELATED STUFF //
    ///////////////////////////
    std::string gamepadMappingsPath = ReLoc::locate(ReType::Any, "gamecontrollerdb.txt");
    std::string gamepadMappings = CppTools::String::readFileIntoString(gamepadMappingsPath);
    glfwUpdateGamepadMappings(gamepadMappings.c_str());
    glfwSetJoystickCallback(GLFW3Utilities::globalGlfwJoystickCallback);
    GLFW3Utilities::initGamepadStatuses();

    ///////////////////////////
    // MONITOR RELATED STUFF //
    ///////////////////////////
    _monitors.clear();
    _monitors = _ListMonitors();
    _nativeVideoModes.clear();
    _SaveMonitorVideoModes();
    glfwSetMonitorCallback(_GlobalGlfwMonitorCallback);

    return result;
}

void WindowFactory<WindowBackend::GLFW3>::TerminateBackend()
{
    _monitors.clear();
    _nativeVideoModes.clear();
    
    glfwTerminate();
}

void WindowFactory<WindowBackend::GLFW3>::SetErrorCallback(const void* callback)
{
    glfwSetErrorCallback(*((ErrorCallbackSignature*)callback));
}

MonitorPtr WindowFactory<WindowBackend::GLFW3>::GetPrimaryMonitor()
{
    GLFW3Monitor* glfw3Monitor = new GLFW3Monitor(glfwGetPrimaryMonitor());
    return GLFW3MonitorPtr(glfw3Monitor);
}

std::map<unsigned int, MonitorPtr> WindowFactory<WindowBackend::GLFW3>::GetMonitors()
{
    return _monitors;
}

void WindowFactory<WindowBackend::GLFW3>::SetMonitorCallback(const void* callback)
{
    _monitorCallback = (MonitorCallbackSignature*) callback;
}

Monitor::VideoMode WindowFactory<WindowBackend::GLFW3>::GetMonitorNativeVideoMode(const MonitorPtr monitor)
{
    if (monitor == nullptr)
        throw std::runtime_error("WindowFactory<GLFW3>: null monitor pointer passed, cannot retrieve video mode.");

    auto it = _nativeVideoModes.find(monitor->id);
    if (it == _nativeVideoModes.end())
        throw std::runtime_error("WindowFactory<GLFW3>: native video mode could not be retrieved for passed monitor.");

    return it->second;
}

GLWindowPtr WindowFactory<WindowBackend::GLFW3>::MakeWindow(const WindowCreationParameters& params)
{
    // Window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,      params.glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,      params.glVersionMinor);
    glfwWindowHint(GLFW_RESIZABLE,                  params.resizable                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY,               params.autoMinimize             ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_DECORATED,                  params.decorated                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE,                    params.visible                  ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED,                  params.maximized                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FLOATING,                   params.alwaysOnTop              ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED,                    params.focused                  ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW,              params.focusOnShow              ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,           params.scaleToMonitor           ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,       params.debug                    ? GL_TRUE : GL_FALSE);

#ifndef EGL_DETECTED
	glfwWindowHint(GLFW_OPENGL_PROFILE,             Window::GLFW3Adapter::getValue(params.glProfile));
#endif//EGL_DETECTED

    // Shared GL context
    GLFWwindow* sharedContextWindow = nullptr;
    if (params.shareContext != nullptr)
    {
        GLFW3WindowPtr glfw3SharedWindow = std::static_pointer_cast<GLFW3Window>(params.shareContext);
        sharedContextWindow = glfw3SharedWindow->_w;
    }

    // Fullscreen
    GLFWmonitor* fullscreenMonitor = nullptr;
    if (params.monitor != nullptr)
    {
        GLFW3MonitorPtr glfw3Monitor = std::static_pointer_cast<GLFW3Monitor>(params.monitor);
        fullscreenMonitor = glfw3Monitor->_m;
    }

    int width = params.width;
    int height = params.height;
    int refreshRate = GLFW_DONT_CARE;
    // Bit depths for red, green and blue buffers
    int framebufferDepth[3] = {8, 8, 8};

    if (params.borderlessFullscreen)
    {
        if (fullscreenMonitor == nullptr)
        {
            fullscreenMonitor = glfwGetPrimaryMonitor();
        }

        GLFW3Monitor* glfw3Monitor = (GLFW3Monitor*) glfwGetMonitorUserPointer(fullscreenMonitor);
        Monitor::VideoMode mode = _nativeVideoModes[glfw3Monitor->id];
        width = mode.width;
        height = mode.height;
        refreshRate = mode.refreshRate;
        framebufferDepth[0] = mode.redBits;
        framebufferDepth[1] = mode.greenBits;
        framebufferDepth[2] = mode.blueBits;
    }

    // Framebuffer hints    
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,    params.transparentFramebuffer   ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_RED_BITS,                   framebufferDepth[0]);
    glfwWindowHint(GLFW_GREEN_BITS,                 framebufferDepth[1]);
    glfwWindowHint(GLFW_BLUE_BITS,                  framebufferDepth[2]);
    glfwWindowHint(GLFW_REFRESH_RATE,               refreshRate);

	// Instantiate window
	GLFWwindow* window = glfwCreateWindow(width, height, params.title.c_str(), fullscreenMonitor, sharedContextWindow);

	if (!window)
	{
        throw std::runtime_error("Failed to create window.");
    }

    // Initialize a GLWindow instance with a GLFWwindow object
    const GLFW3WindowPtr glfw3Window = std::make_shared<GLFW3Window>(window, params.title);
    // Set the user pointer of the GLFWwindow to the newly created GLWindow instance
    const GLWindowPtr glWindow = std::static_pointer_cast<GLWindow>(glfw3Window);
    glfwSetWindowUserPointer(window, (void*)glWindow.get());

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow object and call the appropriate callback on the GLWindow instance
    // Bind all relevant callbacks to such functions
    glfwSetFramebufferSizeCallback(window, GLFW3Utilities::globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, GLFW3Utilities::globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, GLFW3Utilities::globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, GLFW3Utilities::globalGlfwMouseCursorCallback);

    // Plug in joystick events
    GLFW3Utilities::subscribeToGlfwJoystickStatus(glWindow);

    return glWindow;
}

void WindowFactory<WindowBackend::GLFW3>::DestroyWindow(GLWindowPtr window)
{
    GLFW3Utilities::unsubscribeFromGlfwJoystickStatus(window);

    // No need to actually destroy the window, as the underlying GLFW3Window
    // instance will do it in its own destructor when the last pointer to the 
    // instance is released.
}

std::map<unsigned int, MonitorPtr> WindowFactory<WindowBackend::GLFW3>::_ListMonitors()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::map<unsigned int, MonitorPtr> managedMonitors;
    for (int i = 0; i < count; i++)
    {
        GLFW3Monitor* glfw3Monitor = new GLFW3Monitor(monitors[i]);
        GLFW3MonitorPtr glfw3ManagedMonitor = GLFW3MonitorPtr(glfw3Monitor);
        managedMonitors[glfw3Monitor->id] = std::static_pointer_cast<Monitor>(glfw3ManagedMonitor);
    }

    return managedMonitors;
}

void WindowFactory<WindowBackend::GLFW3>::_SaveMonitorVideoModes()
{
    for (const auto& [id, monitor] : _monitors)
    {
        _nativeVideoModes[id] = monitor->getCurrentVideoMode();
    }
}

void WindowFactory<WindowBackend::GLFW3>::_GlobalGlfwMonitorCallback(GLFWmonitor* m, int event)
{
    if (_monitorCallback != nullptr) (*_monitorCallback)(m, event);

    if (event == GLFW_CONNECTED)
    {
        GLFW3Monitor* glfw3Monitor = new GLFW3Monitor(m);
        GLFW3MonitorPtr glfw3ManagedMonitor = GLFW3MonitorPtr(glfw3Monitor);
        _monitors[glfw3Monitor->id] = std::static_pointer_cast<Monitor>(glfw3ManagedMonitor);
    }
    else // if (event == GLFW_DISCONNECTED)
    {
        GLFW3Monitor* monitor = (GLFW3Monitor*)glfwGetMonitorUserPointer(m);
        if (monitor != nullptr)
        {
            _monitors.erase(monitor->id);
            _nativeVideoModes.erase(monitor->id);
        }
    }
}

}//namespace Renderboi::Window
