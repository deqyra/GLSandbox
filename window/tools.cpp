#include "tools.hpp"

#include <cstdlib>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "enums.hpp"
#include "gl_window.hpp"

#include "glfw3/glfw3_window.hpp"
#include "glfw3/glfw3_adapter.hpp"
#include "glfw3/glfw3_window_callbacks.hpp"

GLWindowPtr makeGLFWWindow(std::string title, int width, int height, int glVersionMajor, int glVersionMinor, Window::OpenGLProfile glProfile, bool debug)
{
    static bool glfw3AdapterInitialized = false;
    if (!glfw3AdapterInitialized) Window::GLFW3Adapter::initialize();

    glfw3AdapterInitialized = true;
    
	// GL metadata
	if (glVersionMajor > -1) glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    if (glVersionMinor > -1) glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, Window::GLFW3Adapter::getValue(glProfile));

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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(window);
        throw std::runtime_error("Failed to load GL function pointers.");
    }

    if (debug)
    {
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

    // It is not possible to set methods of a user-defined class as the callbacks of a window, because of function pointer type mismatch
    // Instead, use the user pointer of the GLFWwindow object, which can be set to point to any user-defined window class

    GLWindowPtr glWindow = std::make_shared<GLFW3Window>(window, title); // Initialize a GLWindow instance with a GLFWwindow object
    glfwSetWindowUserPointer(window, glWindow.get());                   // Set the user pointer of the GLFWwindow to the newly created GLWindow instance

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow object and call the appropriate callback on the GLWindow instance
    // Bind all relevant callbacks to such functions
    glfwSetFramebufferSizeCallback(window, globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, globalGlfwMouseCursorCallback);

    // Return a shared pointer
    return glWindow;
}

void simpleErrorCallback(int error, const char* description)
{
	// Print the error
	std::cerr << "Error code: " << error << std::endl;
	std::cerr << "Description: " << description << std::endl;
}

void APIENTRY glDebugOutput(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    // Print error info on the console
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

float glAspectRatio()
{
    int dims[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, dims);
    // dims[0] = viewport position X
    // dims[1] = viewport position Y
    // dims[2] = viewport width
    // dims[3] = viewport height

    return (float)(dims[2]) / (float)(dims[3]);
}
