/**
	GLTest, gl_utils.hpp
	Purpose: Provide boilerplate functions to kick-start GLFW/GLAD/OpenGL development.

	@author François Brachais (deqyra)
	@version 1.0 12/06/2019
 */
#ifndef GL_UTILS_HPP
#define GL_UTILS_HPP

#include <string>

#include "../../include/glad/glad.h"
#include "../../include/GLFW/glfw3.h"

// Compiles GLSL code from a file into a shader.
unsigned int loadShader(GLenum shaderType, std::string filename);

// Combines shaders and links them into a program.
unsigned int makeShaderProgram(unsigned int count...);

// Creates a GL context within a GLFW window, binds callbacks and returns the whole thing. Also initialises GL pointers.
GLFWwindow* makeWindow(std::string name, int width = 640, int height = 480, int glVersionMajor = 4, int glVersionMinor = 6, bool debug = true);

// Destroys the provided GLFW window along with the object managing its callbacks.
void destroyWindow(GLFWwindow* window);

// Output print callback upon getting GL errors
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

// Returns the aspect ratio (width / height) of the current context
float glAspectRatio();

// Shortcut macros
#define SQRT3 1.7320508075688772935274463415059

#define WHITE   1.0f, 1.0f, 1.0f
#define GREY    0.5f, 0.5f, 0.5f
#define BLACK   0.0f, 0.0f, 0.0f
#define RED     1.0f, 0.0f, 0.0f
#define GREEN   0.0f, 1.0f, 0.0f
#define BLUE    0.0f, 0.0f, 1.0f
#define CYAN    0.0f, 1.0f, 1.0f
#define MAGENTA 1.0f, 0.0f, 1.0f
#define YELLOW  1.0f, 1.0f, 0.0f

#endif//GL_UTILS_HPP