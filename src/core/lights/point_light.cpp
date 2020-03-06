#include "point_light.hpp"

const std::function<float(float)> PointLight::linearCoeffFromDesiredRange = [](float range) -> float
{
    return 1.f / ((5.07867e-4 * range * range) + (2.03957e-1 * range));
};

const std::function<float(float)> PointLight::quadraticCoeffFromDesiredRange = [](float range) -> float
{
    return 1.f / ((1.51749e-2 * range * range) + (-2.68263e-2 * range));
};

PointLight::PointLight() :
    PointLight(50.f)
{

}

PointLight::PointLight(float range) :
    PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range)
{

}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range) :
    Light(LightType::PointLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{
    constant = 1.f;
    linear = linearCoeffFromDesiredRange(range);
    quadratic = quadraticCoeffFromDesiredRange(range);
}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) :
    Light(LightType::PointLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    constant(constant),
    linear(linear),
    quadratic(quadratic)
{

}