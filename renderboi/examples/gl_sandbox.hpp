#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_HPP

#include <memory>
#include <string>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/gl_context_client.hpp>

#include "gl_sandbox_parameters.hpp"

namespace Renderboi
{

/// @brief Abstract class to run any 3D scene in a GL window.
class GLSandbox : public Window::GLContextClient,
				  public std::enable_shared_from_this<GLSandbox>
{
	public:
		/// @brief Set up the window prior to running the example. Will be
		/// called from the main thread.
		///
		/// @param window Pointer to the window to initialize.
		virtual void setUp() = 0;

        /// @brief Run something in the provided GL window. To be executed by
		/// a separate thread.
		///
		/// @param window Pointer to the window to run stuff in.
		virtual void run() = 0;

		/// @brief Restore the window back to how it was before the example ran.
		/// The contents of this function should be the opposite from those in
		/// setUp(). Will be called from the main thread once run() has returned.
		///
		/// @param window Pointer to the window to detach from.
		virtual void tearDown() = 0;

	protected:
		/// @param window Pointer to the window on which the sandbox should run.
        /// @param params Strcture packing the parameters according to which the
        /// sandbox should run.
		GLSandbox(const GLWindowPtr window, const GLSandboxParameters params);

		/// @brief Initialize an OpenGL context for the sandbox.
		virtual void _initContext();

		/// @brief Release the OpenGL context of the sandbox.
		virtual void _terminateContext();

		/// @brief Structure packing the parameters according to which the 
		/// sandbox should run.
		GLSandboxParameters _parameters;
};

}//namespace Renderboi

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_HPP