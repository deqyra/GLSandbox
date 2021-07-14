#include "gl_sandbox.hpp"

#include <glad/gl.h>

#include <renderboi/utilities/gl_utilities.hpp>

namespace Renderboi
{

GLSandbox::GLSandbox(const GLWindowPtr window, const GLSandboxParameters params) :
    GLContextClient(window),
    _parameters(params)
{

}

void GLSandbox::_initContext()
{
    static Window::GLContextClientPtr thisContextClient =
    static_pointer_cast<Window::GLContextClient>(this->shared_from_this());

    _window->makeContextCurrent(thisContextClient);

    if (_parameters.debug)
    {
        if (!_window->extensionSupported("GL_ARB_debug_output"))
        {
            throw std::runtime_error("A debug context was requested but GL_ARB_debug_output is not available.");
        }

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(glDebugCallback, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void GLSandbox::_terminateContext()
{
    _window->releaseContext();
}

}
