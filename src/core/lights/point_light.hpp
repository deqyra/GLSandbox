#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <glm/glm.hpp>

#include "../positioned_object.hpp"
#include "../light.hpp"
#include "../light_type.hpp"

#define POINT_LIGHT_MAX_COUNT 128
#define POINT_LIGHT_UBO_SIZE  64

class PointLight : public Light
{
    public:
        PointLight();
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};

#endif//POINT_LIGHT_HPP