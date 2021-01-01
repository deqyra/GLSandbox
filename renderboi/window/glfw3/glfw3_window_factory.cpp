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

#include <renderboi/utilities/gl_utilities.hpp>

#include "../env_info.hpp"
#include "../enums.hpp"
#include "../gl_window.hpp"
#include "../window_backend.hpp"

#include "glfw3_window.hpp"
#include "glfw3_adapter.hpp"
#include "glfw3_window_callbacks.hpp"

int WindowFactory<WindowBackend::GLFW3>::InitializeBackend()
{
    return glfwInit();
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

	// Init context in new window
	glfwMakeContextCurrent(window);

	// Load GL pointers
	if (!gladLoadGL())
	{
		glfwDestroyWindow(window);
        throw std::runtime_error("Failed to load GL function pointers.");
    }

    if (debug)
    {
        if (!glfwExtensionSupported("GL_ARB_debug_output"))
        {
    		glfwDestroyWindow(window);
            throw std::runtime_error("A debug context was requested but GL_ARB_debug_output is not available.");
        }

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(glDebugCallback, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // It is not possible to set methods of a user-defined class as the callbacks of a window, because of function pointer type mismatch
    // Instead, use the user pointer of the GLFWwindow object, which can be set to point to any user-defined window class

    const GLWindowPtr glWindow = std::make_shared<GLFW3Window>(window, title); // Initialize a GLWindow instance with a GLFWwindow object
    glfwSetWindowUserPointer(window, glWindow.get());                    // Set the user pointer of the GLFWwindow to the newly created GLWindow instance

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow object and call the appropriate callback on the GLWindow instance
    // Bind all relevant callbacks to such functions
    glfwSetFramebufferSizeCallback(window, globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, globalGlfwMouseCursorCallback);

    // Return a shared pointer
    return glWindow;
}
