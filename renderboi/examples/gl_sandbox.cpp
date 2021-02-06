#include "gl_sandbox.hpp"

#include <glad/gl.h>

#include <renderboi/utilities/gl_utilities.hpp>

namespace Renderboi
{

void GLSandbox::initContext(const GLWindowPtr window, const GLSandboxParameters& params)
{
    window->makeContextCurrent();

    if (params.debug)
    {
        if (!window->extensionSupported("GL_ARB_debug_output"))
        {
            throw std::runtime_error("A debug context was requested but GL_ARB_debug_output is not available.");
        }

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(glDebugCallback, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void GLSandbox::terminateContext(const GLWindowPtr window)
{
    window->releaseContext();
}

}
