#ifndef EXAMPLES__GL_SANDBOX_HPP
#define EXAMPLES__GL_SANDBOX_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

// Abstract class to run any 3D scene in a GLFW window
class GLSandbox
{
	public:
        // Run something in the provided GLFW window
		virtual void run(GLFWwindow* window) = 0;
};

#endif//EXAMPLES__GL_SANDBOX_HPP