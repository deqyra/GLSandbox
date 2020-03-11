#include "gl_utils.hpp"

#include <cstdlib>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "gl_window.hpp"
#include "glfw_window_callbacks.hpp"

#define INFO_BUFFER_SIZE 512

unsigned int loadShader(unsigned int shaderType, std::string filename)
{
	// Open input file and read all of its contents.
	std::ifstream file(filename);

	if (!file.is_open())
		std::cerr << "Shader \"" << filename << "\" could not be found." << std::endl;

	std::string all("");
	std::string line;
	while (std::getline(file, line))
		all += line + "\n";

	const char* source = all.c_str();

	// Compile into shader
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, INFO_BUFFER_SIZE, nullptr, info);
		std::cerr << "Shader compilation \"" << filename << "\" failed:\n" << info << std::endl;
		return 0;
	}

	return shader;
}

// There must be `count` arguments after `count`, all of type `unsigned int`.
unsigned int makeShaderProgram(unsigned int count...)
{
	unsigned int program = glCreateProgram();

	// Iterate through arguments.
	va_list args;
	va_start(args, count);

	for (unsigned int i = 0; i < count; i++)
	{
		// Attach all provided shaders
		unsigned int shader = va_arg(args, unsigned int);
		glAttachShader(program, shader);
	}

	va_end(args);

	// Link all shaders
	glLinkProgram(program);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info);
		std::cerr << "Shader linking failed:\n" << info << std::endl;
		return 0;
	}

	return program;
}

GLFWwindow* makeWindow(std::string name, int width, int height, int glVersionMajor, int glVersionMinor, bool debug)
{
	// GL metadata
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (debug)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Instantiate window
	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

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

    GLWindow* glWindow = new GLWindow(window);          // Initialize a GLWindow instance with a GLFWwindow object
    glfwSetWindowUserPointer(window, glWindow);         // Set the user pointer of the GLFWwindow to the newly created GLWindow instance

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow object and call the appropriate callback on the GLWindow instance
    // Bind all relevant callbacks to such functions
    glfwSetFramebufferSizeCallback(window, globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, globalGlfwMouseCursorCallback);

    return window;
}

void destroyWindow(GLFWwindow* window)
{
    // Destroy the GLWindow object associated with the GLFWwindow object
    GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    delete glWindow;
    glfwDestroyWindow(window);
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
