#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

// Collection of parameters representing a directional light
class DirectionalLight : public Light
{
    public:
        DirectionalLight(glm::vec3 direction);
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        // Get a raw pointer to a new DirectionalLight instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        DirectionalLight* clone();

        // Direction the spot light is facing
        glm::vec3 direction;
        // RGB color of the ambient component of the emitted light
        glm::vec3 ambient;
        // RGB color of the diffuse component of the emitted light
        glm::vec3 diffuse;
        // RGB color of the specular component of the emitted light
        glm::vec3 specular;
};

#endif//DIRECTIONAL_LIGHT_HPP