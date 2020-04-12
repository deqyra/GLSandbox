#ifndef CORE__LIGHTS__SPOT_LIGHT_HPP
#define CORE__LIGHTS__SPOT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

class SpotLight : public Light
{
    public:
        static constexpr float DefaultSpotLightRange = 50.f;
        static constexpr float DefaultSpotLightInnerCutoff = glm::radians(15.f);
        static constexpr float DefaultSpotLightOuterCutoff = glm::radians(20.f);

        static const std::function<float(float)> linearCoeffFromDesiredRange;
        static const std::function<float(float)> quadraticCoeffFromDesiredRange;

        SpotLight(glm::vec3 direction);
        SpotLight(glm::vec3 direction, float range);
        SpotLight(glm::vec3 direction, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, float range, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range, float innerCutoff, float outerCutoff);
        SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutoff, float outerCutoff);

        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
        float innerCutoff;
        float outerCutoff;
};

#endif//CORE__LIGHTS__SPOT_LIGHT_HPP