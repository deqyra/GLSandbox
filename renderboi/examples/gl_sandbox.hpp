#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_HPP

#include <string>

#include <renderboi/window/gl_window.hpp>

/// @brief Abstract class to run any 3D scene in a GL window.
class GLSandbox
{
	public:
        /// @brief Run something in the provided GL window.
		///
		/// @param window Pointer to the window to run stuff in.
		virtual void run(GLWindowPtr window) = 0;
};

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_HPP