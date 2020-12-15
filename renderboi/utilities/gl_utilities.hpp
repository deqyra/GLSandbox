#ifndef RENDERBOI__UTILITIES__GL_UTILITIES_HPP
#define RENDERBOI__UTILITIES__GL_UTILITIES_HPP

#include <glad/gl.h>

void APIENTRY glDebugCallback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam);

float glGetAspectRatio();

#endif//RENDERBOI__UTILITIES__GL_UTILITIES_HPP