#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

// Collection of parameters representing a single point light
class PointLight : public Light
{
    public:
        static constexpr float DefaultRange = 50.f;

        PointLight();
        PointLight(float range);
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range);
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

        // RGB color of the ambient component of the emitted light
        glm::vec3 ambient;
        // RGB color of the diffuse component of the emitted light
        glm::vec3 diffuse;
        // RGB color of the specular component of the emitted light
        glm::vec3 specular;

        // Constant term of the light attenuation computation
        float constant;
        // Linear coefficient of the light attenuation computation
        float linear;
        // Quadratic coefficient of the light attenuation computation
        float quadratic;

        // Get a raw pointer to a new PointLight instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        PointLight* clone();
        // Recompute light attenuation factors according to a new range
        void setRange(float range);
};

#endif//POINT_LIGHT_HPP