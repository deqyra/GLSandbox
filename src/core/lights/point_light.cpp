#include "point_light.hpp"

const std::function<float(float)> PointLight::linearCoeffFromDesiredRange = [](float range) -> float
{
    return (2.041118599e-7 * range * range) - (7.382441708e-4 * range) + 2.481753022e-1;
};

const std::function<float(float)> PointLight::quadraticCoeffFromDesiredRange = [](float range) -> float
{
    return (3.682978425e-7 * range * range) - (1.318877109e-3 * range) + 4.032644093e-1;
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