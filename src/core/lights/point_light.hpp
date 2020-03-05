#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "../light.hpp"
#include "../light_type.hpp"

#define POINT_LIGHT_MAX_COUNT 64
#define POINT_LIGHT_UBO_SIZE  80

class PointLight : public Light
{
    public:
        PointLight();
        PointLight(float range);
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range);
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        static const std::function<float(float)> linearCoeffFromDesiredRange;
        static const std::function<float(float)> quadraticCoeffFromDesiredRange;
};

#endif//POINT_LIGHT_HPP