#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

class DirectionalLight : public Light
{
    public:
        DirectionalLight(glm::vec3 direction);
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};

#endif//DIRECTIONAL_LIGHT_HPP