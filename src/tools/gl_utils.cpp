/**
	GLTest, gl_utils.cpp
	Purpose: Implementation of functions declared in gl_utils.hpp.

	@author François Brachais (deqyra)
	@version 1.0 12/06/2019
 */
#include "gl_utils.hpp"

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::runtime_error;
using std::string;

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "gl_window.hpp"

#define INFO_BUFFER_SIZE 512

unsigned int loadShader(GLenum shaderType, string filename)
{
	// Open input file and read all of its contents.
	ifstream file(filename);

	if (!file.is_open())
		cerr << "Shader \"" << filename << "\" could not be found." << endl;

	string all("");
	string line;
	while (getline(file, line))
		all += line + "\n";

	// Cast to a const C string
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
		cerr << "Shader compilation \"" << filename << "\" failed:\n" << info << endl;
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
		cerr << "Shader linking failed:\n" << info << endl;
		return 0;
	}

	return program;
}

GLFWwindow* makeWindow(string name, int width, int height, int glVersionMajor, int glVersionMinor, bool debug)
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
        throw runtime_error("Failed to create window.");
    }

	// Init context in new window
	glfwMakeContextCurrent(window);

	// Load GL pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(window);
        throw runtime_error("Failed to load GL function pointers.");
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

    // It is not possible to set the callbacks of a window as the methods of user-defined class, because of function pointer type mismatch.
    // A workaround is to use the user pointer of the GLFWwindow object, which can be set to point to any user-defined window class.
    // This way, an instance of the user-defined window class is effectively bound both ways to a GLFWwindow instance (assuming the user-defined class properly stores a reference to a GLFWwindow object).

    GLWindow* glWindow = new GLWindow(window);          // Initialise a user-defined window instance with a GLFWwindow instance.
    glfwSetWindowUserPointer(window, glWindow);         // Set the user pointer of the GLFWwindow to the newly created user-defined window instance.

    // Then, a lambda taking the same parameters as a callback is declared.
    // In the body of the lambda, the user-defined window pointer is retrieved from the GLFWwindow instance, and is used to call the user-defined class methods with the lambda parameters.
    // That whole lambda expression, matching the expected function pointer type of the callback, can be set as the actual callback for the initial GLFWwindow instance.

    // Window resized callback
    auto framebufferResizeCallback = [](GLFWwindow* window, int width, int height)                                      // Declare a lambda
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processFramebufferResize(window, width, height);     // Retrieve the user pointer of the window, call the appropriate method.
    };
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);                                                  // Statically bind the lambda as a callback.

    // Keyboard callback
    auto keyboardCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processKeyboard(window, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, keyboardCallback);

    // Mouse click callback
    auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseButton(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Mouse cursor callback
    auto mouseCursorCallback = [](GLFWwindow* window, double xpos, double ypos)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->processMouseCursor(window, xpos, ypos);
    };
    glfwSetCursorPosCallback(window, mouseCursorCallback);

    return window;
}

void destroyWindow(GLFWwindow* window)
{
    GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    delete glWindow;
    glfwDestroyWindow(window);
}

void simpleErrorCallback(int error, const char* description)
{
	// Print the error.
	cerr << "Error code: " << error << endl;
	cerr << "Description: " << description << endl;
}

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // Ignore non-significant error/warning codes.
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    cout << "---------------" << endl;
    cout << "Debug message (" << id << "): " << message << endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           cout << "Source: Other"; break;
    } cout << endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               cout << "Type: Other"; break;
    } cout << endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: cout << "Severity: notification"; break;
    } cout << endl;
    cout << endl;
}

float glAspectRatio()
{
    int dims[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, dims);
    return (float)(dims[2]) / (float)(dims[3]);
}
