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

unsigned int LightUBO::getBindingPoint()
{
    return LIGHT_UBO_BINDING_POINT;
}

unsigned int LightUBO::getSize()
{
    return LIGHT_UBO_SIZE;
}

void LightUBO::setPoint(unsigned int index, PointLight point, glm::vec3 position)
{
    unsigned int offset = POINT_LIGHT_UBO_OFFSET + (index * POINT_LIGHT_UBO_SIZE);

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec3), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec3), glm::value_ptr(point.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec3), glm::value_ptr(point.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec3), glm::value_ptr(point.specular));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 64, sizeof(float), &point.constant);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 68, sizeof(float), &point.linear);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 72, sizeof(float), &point.quadratic);
}

void LightUBO::setPointCount(unsigned int count)
{
    unsigned int offset = LIGHT_UBO_COUNTS_OFFSET;

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}

void LightUBO::setSpot(unsigned int index, SpotLight spot, glm::vec3 position)
{
    unsigned int offset = SPOT_LIGHT_UBO_OFFSET + (index * SPOT_LIGHT_UBO_SIZE);

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec3), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec3), glm::value_ptr(spot.direction));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec3), glm::value_ptr(spot.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec3), glm::value_ptr(spot.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 64, sizeof(glm::vec3), glm::value_ptr(spot.specular));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 80, sizeof(float), &spot.constant);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 84, sizeof(float), &spot.linear);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 88, sizeof(float), &spot.quadratic);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 92, sizeof(float), &spot.innerCutoff);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 96, sizeof(float), &spot.outerCutoff);
}

void LightUBO::setSpotCount(unsigned int count)
{
    unsigned int offset = LIGHT_UBO_COUNTS_OFFSET + 4;

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}

void LightUBO::setDirectional(unsigned int index, DirectionalLight direct)
{
    unsigned int offset = DIRECTIONAL_LIGHT_UBO_OFFSET + (index * DIRECTIONAL_LIGHT_UBO_SIZE);

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec3), glm::value_ptr(direct.direction));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec3), glm::value_ptr(direct.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec3), glm::value_ptr(direct.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec3), glm::value_ptr(direct.specular));
}

void LightUBO::setDirectionalCount(unsigned int count)
{
    unsigned int offset = LIGHT_UBO_COUNTS_OFFSET + 8;

    glBindBuffer(GL_UNIFORM_BUFFER, _id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}
