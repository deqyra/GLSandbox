#ifndef CORE__LIGHTS__SPOT_LIGHT_HPP
#define CORE__LIGHTS__SPOT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

// Collection of parameters representing a single spot light
class SpotLight : public Light
{
    public:
        static constexpr float DefaultRange = 50.f;
        static constexpr float DefaultInnerCutoff = glm::radians(15.f);
        static constexpr float DefaultOuterCutoff = glm::radians(20.f);

        SpotLight(glm::vec3 direction);
        SpotLight(glm::vec3 direction, float range);
        SpotLight(glm::vec3 direction, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, float range, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutoff, float outerCutoff);

        // Direction the spot light is facing
        glm::vec3 direction;
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
        // Angle at which the light starts fading out
        float innerCutoff;
        // Angle at which the light finishes fading out
        float outerCutoff;

        // Get a raw pointer to a new SpotLight instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        SpotLight* clone();
        // Recompute light attenuation factors according to a new range
        void setRange(float range);
};

#endif//CORE__LIGHTS__SPOT_LIGHT_HPP