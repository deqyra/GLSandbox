#include "matrix_ubo.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

#include "ubo_info.hpp"

MatrixUBO::MatrixUBO()
{
    // Generate the buffer and allocate space
    glGenBuffers(1, &_location);
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferData(GL_UNIFORM_BUFFER, getSize(), NULL, GL_STATIC_DRAW);

    // Bind to binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, getBindingPoint(), _location); 
}

unsigned int MatrixUBO::getBindingPoint()
{
    return MATRIX_UBO_BINDING_POINT;
}

unsigned int MatrixUBO::getSize()
{
    return 256;
}

void MatrixUBO::setModel(glm::mat4 model)
{
    // Send the matrix on the GPU memory, with an appropriate offset
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(model));
}

void MatrixUBO::setView(glm::mat4 view)
{
    // Send the matrix on the GPU memory, with an appropriate offset
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), glm::value_ptr(view));
}

void MatrixUBO::setProjection(glm::mat4 projection)
{
    // Send the matrix on the GPU memory, with an appropriate offset
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(glm::mat4), glm::value_ptr(projection));
}

void MatrixUBO::setNormal(glm::mat4 normal)
{
    // Send the matrix on the GPU memory, with an appropriate offset
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, 192, sizeof(glm::mat4), glm::value_ptr(normal));
}
