#include "gl_sandbox.hpp"

#include <glad/gl.h>

#include <renderboi/utilities/gl_utilities.hpp>

namespace Renderboi
{

GLSandbox::GLSandbox(const GLWindowPtr window) :
    GLContextClient(window)
{

}

void GLSandbox::initContext(const GLSandboxParameters& params)
{
    static Window::GLContextClientPtr thisContextClient =
    static_pointer_cast<Window::GLContextClient>(this->shared_from_this());

    _window->makeContextCurrent();
    _window->glContextClient = thisContextClient;

    if (params.debug)
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

void GLSandbox::terminateContext()
{
    _window->glContextClient = nullptr;
    _window->releaseContext();
}

}
