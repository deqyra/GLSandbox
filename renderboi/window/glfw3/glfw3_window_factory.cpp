#include "../window_factory.hpp"
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

#include "../env_info.hpp"
#include "../enums.hpp"
#include "../gl_window.hpp"
#include "../window_backend.hpp"

#include "glfw3_window.hpp"
#include "glfw3_adapter.hpp"
#include "glfw3_utilities.hpp"

namespace Renderboi
{

using ReLoc = ResourceLocator;
using ReType = ResourceType;

int WindowFactory<WindowBackend::GLFW3>::InitializeBackend()
{
    int result = glfwInit();
    if (!result) return result;

    ///////////////////////////
    // GAMEPAD RELATED STUFF //
    ///////////////////////////
    std::string gamepadMappingsPath = ReLoc::locate(ReType::Any, "gamecontrollerdb.txt");
    std::string gamepadMappings = CppTools::String::readFileIntoString(gamepadMappingsPath);
    glfwUpdateGamepadMappings(gamepadMappings.c_str());
    glfwSetJoystickCallback(globalGlfwJoystickCallback);
    pollGamepads();

    return result;
}

void WindowFactory<WindowBackend::GLFW3>::TerminateBackend()
{
    glfwTerminate();
}

void WindowFactory<WindowBackend::GLFW3>::SetErrorCallback(const void* callback)
{
    glfwSetErrorCallback(*((ErrorCallbackSignature*)callback));
}

GLWindowPtr WindowFactory<WindowBackend::GLFW3>::MakeWindow(std::string title, int width, int height, int glVersionMajor, int glVersionMinor, Window::OpenGLProfile glProfile, bool debug)
{
	// GL metadata
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    
#ifndef EGL_DETECTED
	glfwWindowHint(GLFW_OPENGL_PROFILE, Window::GLFW3Adapter::getValue(glProfile));
#endif//EGL_DETECTED

    if (debug)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Instantiate window
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!window)
	{
        throw std::runtime_error("Failed to create window.");
    }

    // Initialize a GLWindow instance with a GLFWwindow object
    const GLFW3WindowPtr glfw3Window = std::make_shared<GLFW3Window>(window, title);
    // Set the user pointer of the GLFWwindow to the newly created GLWindow instance
    const GLWindowPtr glWindow = std::static_pointer_cast<GLWindow>(glfw3Window);
    glfwSetWindowUserPointer(window, (void*)glWindow.get());

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow
    // object and call the appropriate callback on the GLWindow instance.
    // Bind all relevant callbacks to such functions.
    glfwSetFramebufferSizeCallback(window, globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, globalGlfwMouseCursorCallback);

    subscribeToGlfwJoystickStatus(glWindow);

    return glWindow;
}

void WindowFactory<WindowBackend::GLFW3>::DestroyWindow(GLWindowPtr window)
{
    unsubscribeFromGlfwJoystickStatus(window);

    // No need to actually destroy the window, as the underlying GLFW3Window
    // instance will do it in its own destructor.
}

}//namespace Renderboi
