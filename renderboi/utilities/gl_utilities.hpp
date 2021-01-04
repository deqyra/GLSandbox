#ifndef RENDERBOI__UTILITIES__GL_UTILITIES_HPP
#define RENDERBOI__UTILITIES__GL_UTILITIES_HPP

#include <glad/gl.h>

namespace Renderboi
{
// The "naming convention" used hereafter is a cheat to make these funciton feel
// like they are part of the GL API, although they really are not.

/// @brief Have glDebugCallback ignore messages of a certain type. Subsequent 
/// calls result in several message types being ignored. Use 
/// `glStopIgnoringDebugMessages` to undo.
///
/// @param type GL constant corresponding to the debug message type to ignore.
void glIgnoreDebugMessagesOfType(int type);

/// @brief Have glDebugCallback stop ignoring certain message types as a result
/// of calls to `glIgnoreDebugMessagesOfType`.
void glStopIgnoringDebugMessages();

void APIENTRY glDebugCallback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam
);

float glGetAspectRatio();
}//namespace Renderboi

#endif//RENDERBOI__UTILITIES__GL_UTILITIES_HPP