#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "../light.hpp"
#include "../light_type.hpp"

#define SPOT_LIGHT_MAX_COUNT 64
#define SPOT_LIGHT_UBO_SIZE  112

#define DEFAULT_SPOT_LIGHT_RANGE 50.f
#define DEFAULT_SPOT_LIGHT_INNER_CUTOFF 15.f
#define DEFAULT_SPOT_LIGHT_OUTER_CUTOFF 20.f

class SpotLight : public Light
{
    public:
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

        float innerCutoff;
        float outerCutoff;
        float constant;
        float linear;
        float quadratic;

        static const std::function<float(float)> linearCoeffFromDesiredRange;
        static const std::function<float(float)> quadraticCoeffFromDesiredRange;
};

#endif//SPOT_LIGHT_HPP