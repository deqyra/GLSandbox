#include "point_light.hpp"

#include "tools.hpp"

PointLight::PointLight() :
    PointLight(DefaultRange)
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
    attenuationFromRange(range, constant, linear, quadratic);
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

PointLight* PointLight::clone()
{
    return new PointLight(ambient, diffuse, specular, constant, linear, quadratic);
}

void PointLight::setRange(float range)
{
    attenuationFromRange(range, constant, linear, quadratic);
}
