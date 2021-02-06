#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_HPP

#include <string>

#include <renderboi/window/gl_window.hpp>

#include "gl_sandbox_parameters.hpp"

namespace Renderboi
{

/// @brief Abstract class to run any 3D scene in a GL window.
class GLSandbox
{
	public:
		/// @brief Set up the window prior to running the example. Will be
		/// called from the main thread.
		///
		/// @param window Pointer to the window to initialize.
		virtual void setUp(const GLWindowPtr window, const GLSandboxParameters& params) = 0;

        /// @brief Run something in the provided GL window. To be executed by
		/// a separate thread.
		///
		/// @param window Pointer to the window to run stuff in.
		virtual void run(const GLWindowPtr window, const GLSandboxParameters& params) = 0;

		/// @brief Restore the window back to how it was before the example ran.
		/// The contents of this function should be the opposite from those in
		/// setUp(). Will be called from the main thread once run() has returned.
		///
		/// @param window Pointer to the window to detach from.
		virtual void tearDown(const GLWindowPtr window) = 0;

	protected:
		/// @brief Initialize an OpenGL context for the sandbox.
		virtual void initContext(const GLWindowPtr window, const GLSandboxParameters& params);

		/// @brief Release the OpenGL context of the sandbox.
		virtual void terminateContext(const GLWindowPtr window);
};

}//namespace Renderboi

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_HPP