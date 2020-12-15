#include "gl_utilities.hpp"

#include <iostream>
#include <unordered_set>

#include <glad/gl.h>

static std::unordered_set<int> ignoredMessageTypes;

void glIgnoreDebugMessagesOfType(int type)
{
    ignoredMessageTypes.insert(type);
}

void glStopIgnoringDebugMessages()
{
    ignoredMessageTypes.clear();
}

void APIENTRY glDebugCallback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    // Suppress ignored messages
    if (ignoredMessageTypes.find(type) != ignoredMessageTypes.end()) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    // Print error info on the console
    switch (source)
    {
        case GL_DEBUG_SOURCE_API_ARB:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER_ARB:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR_ARB:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:         std::cout << "Type: Performance"; break;
        // case GL_DEBUG_TYPE_MARKER:               std::cout << "Type: Marker"; break;
        // case GL_DEBUG_TYPE_PUSH_GROUP:           std::cout << "Type: Push Group"; break;
        // case GL_DEBUG_TYPE_POP_GROUP:            std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER_ARB:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW_ARB:          std::cout << "Severity: low"; break;
        // case GL_DEBUG_SEVERITY_NOTIFICATION:  std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

float glGetAspectRatio()
{
    int dims[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, dims);
    // dims[0] = viewport position X
    // dims[1] = viewport position Y
    // dims[2] = viewport width
    // dims[3] = viewport height

    return (float)(dims[2]) / (float)(dims[3]);
}
