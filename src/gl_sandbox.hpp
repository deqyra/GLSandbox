#ifndef GL_SANDBOX_HPP
#define GL_SANDBOX_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"

// Abstract class to run any 3D scene in a GLFW window
class GLSandbox
{
	public:
        // Run something in the provided GLFW window
		virtual void run(GLFWwindow* window) = 0;
};

#endif//GL_SANDBOX_HPP