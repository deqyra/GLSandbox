/**
	GLTest, GLSandbox.cpp
	Purpose: Abstract class declaration for generic OpenGL examples.

	@author François Brachais (deqyra)
	@version 1.0 12/06/2019
 */
#ifndef GL_EXAMPLE_HPP
#define GL_EXAMPLE_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"

class GLSandbox
{
	public:
		virtual void run(GLFWwindow* window) = 0;
};

#endif//GL_EXAMPLE_HPP