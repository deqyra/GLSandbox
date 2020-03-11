#ifndef TOOLS__GL_UTILS_HPP
#define TOOLS__GL_UTILS_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include "../../include/GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

// Compile GLSL code from a file into a shader
unsigned int loadShader(unsigned int shaderType, std::string filename);

// Combine shaders and link them into a program
unsigned int makeShaderProgram(unsigned int count...);

// Create a GL context within a GLFW window, bind callbacks and initialize GL function pointers
GLFWwindow* makeWindow(std::string name, int width = 640, int height = 480, int glVersionMajor = 4, int glVersionMinor = 6, bool debug = true);

// Destroy the provided GLFW window along with the object managing its callbacks
void destroyWindow(GLFWwindow* window);

// Output callback, called upon getting GL errors
void APIENTRY glDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

// Return the aspect ratio (width / height) of the current context
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

#endif//TOOLS__GL_UTILS_HPP