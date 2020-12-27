#include "light_ubo.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

LightUBO::LightUBO()
{
    // Generate the buffer and allocate space
    glGenBuffers(1, &_location);
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferData(GL_UNIFORM_BUFFER, Size, NULL, GL_STATIC_DRAW);

    // Bind to binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, _location); 
}

void LightUBO::setPoint(const unsigned int index, const PointLight& point, const glm::vec3& position)
{
    unsigned int offset = PointLightOffset + (index * PointLightSize);

    // Send the point light data to the appropriate memory location on the GPU
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec3), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec3), glm::value_ptr(point.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec3), glm::value_ptr(point.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec3), glm::value_ptr(point.specular));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 64, sizeof(float), &point.constant);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 68, sizeof(float), &point.linear);
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 72, sizeof(float), &point.quadratic);
}

void LightUBO::setPointCount(const unsigned int count)
{
    unsigned int offset = LightCountsOffset;

    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}

void LightUBO::setSpot(const unsigned int index, const SpotLight& spot, const glm::vec3& position)
{
    unsigned int offset = SpotLightOffset + (index * SpotLightSize);

    // Send the spot light data to the appropriate memory location on the GPU
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
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

void LightUBO::setSpotCount(const unsigned int count)
{
    unsigned int offset = LightCountsOffset + 4;

    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}

void LightUBO::setDirectional(const unsigned int index, const DirectionalLight& direct)
{
    unsigned int offset = DirectionalLightOffset + (index * DirectionalLightSize);

    // Send the directional light data to the appropriate memory location on the GPU
    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, offset +  0, sizeof(glm::vec3), glm::value_ptr(direct.direction));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, sizeof(glm::vec3), glm::value_ptr(direct.ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 32, sizeof(glm::vec3), glm::value_ptr(direct.diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offset + 48, sizeof(glm::vec3), glm::value_ptr(direct.specular));
}

void LightUBO::setDirectionalCount(const unsigned int count)
{
    unsigned int offset = LightCountsOffset + 8;

    glBindBuffer(GL_UNIFORM_BUFFER, _location);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &count);
}
