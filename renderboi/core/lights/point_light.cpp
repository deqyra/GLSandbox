#include "point_light.hpp"

#include "tools.hpp"

namespace Renderboi
{

PointLight::PointLight(float range) :
    PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range)
{

}

PointLight::PointLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const float range) :
    Light(LightType::PointLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{
    attenuationFromRange(range, constant, linear, quadratic);
}

PointLight::PointLight(
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular,
    const float constant,
    const float linear,
    const float quadratic
) :
    Light(LightType::PointLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    constant(constant),
    linear(linear),
    quadratic(quadratic)
{

}

void PointLight::setRange(float range)
{
    attenuationFromRange(range, constant, linear, quadratic);
}

PointLight* PointLight::clone() const
{
    return new PointLight(ambient, diffuse, specular,
                          constant, linear, quadratic);
}

}//namespace Renderboi
