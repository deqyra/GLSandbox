#include "matrix_ubo.hpp"
#include "ubo_info.hpp"
#include "../../tools/gl_utils.hpp"

#include <glm/gtc/type_ptr.hpp>

MatrixUBO::MatrixUBO()
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferData(GL_UNIFORM_BUFFER, getSize(), NULL, GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, getBindingPoint(), _id); 
}

void MatrixUBO::setModel(glm::mat4 model)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(model));
}

void MatrixUBO::setView(glm::mat4 view)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), glm::value_ptr(view));
}

void MatrixUBO::setProjection(glm::mat4 projection)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(glm::mat4), glm::value_ptr(projection));
}

void MatrixUBO::setNormal(glm::mat4 normal)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, 192, sizeof(glm::mat4), glm::value_ptr(normal));
}

unsigned int MatrixUBO::getBindingPoint()
{
    return MATRIX_UBO_BINDING_POINT;
}

unsigned int MatrixUBO::getSize()
{
    return 256;
}
