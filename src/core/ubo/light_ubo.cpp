#include "light_ubo.hpp"
#include "ubo_info.hpp"
#include "../../tools/gl_utils.hpp"

#include <glm/gtc/type_ptr.hpp>

LightUBO::LightUBO()
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferData(GL_UNIFORM_BUFFER, getSize(), NULL, GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, getBindingPoint(), _id); 
}

void LightUBO::setPoint(unsigned int index, PointLight point)
{
    unsigned int offset = POINT_LIGHT_UBO_SIZE * index;

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec4), glm::value_ptr(point.getPosition()));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec4), glm::value_ptr(point.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec4), glm::value_ptr(point.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec4), glm::value_ptr(point.specular));
}

void LightUBO::setPointCount(unsigned int count)
{
    unsigned int offset = POINT_LIGHT_UBO_SIZE * POINT_LIGHT_MAX_COUNT;

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}

unsigned int LightUBO::getBindingPoint()
{
    return LIGHT_UBO_BINDING_POINT;
}

unsigned int LightUBO::getSize()
{
    return (POINT_LIGHT_UBO_SIZE * POINT_LIGHT_MAX_COUNT) + sizeof(unsigned int);
}
