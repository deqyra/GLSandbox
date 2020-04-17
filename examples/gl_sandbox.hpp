#ifndef EXAMPLES__GL_SANDBOX_HPP
#define EXAMPLES__GL_SANDBOX_HPP

#include <string>

#include "../windowing/gl_window.hpp"

// Abstract class to run any 3D scene in a GLFW window
class GLSandbox
{
	public:
        // Run something in the provided GLFW window
		virtual void run(GLWindowPtr window) = 0;
};

#endif//EXAMPLES__GL_SANDBOX_HPP