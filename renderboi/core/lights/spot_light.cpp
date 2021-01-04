#include "spot_light.hpp"
#include "tools.hpp"

namespace Renderboi
{

SpotLight::SpotLight(const glm::vec3 direction) :
    SpotLight(direction, DefaultRange)
{

}

SpotLight::SpotLight(const glm::vec3 direction, const float range) :
    SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range)
{

}

SpotLight::SpotLight(const glm::vec3 direction, const float innerCutoff, const float outerCutoff) :
	SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(const glm::vec3 direction, const float range, const float innerCutoff, const float outerCutoff) :
	SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range, innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular) :
	SpotLight(direction, ambient, diffuse, specular, DefaultRange, DefaultInnerCutoff, DefaultOuterCutoff)
{

}

SpotLight::SpotLight(
    const glm::vec3 direction,
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular,
    const float range
) :
	SpotLight(direction, ambient, diffuse, specular, range, DefaultInnerCutoff, DefaultOuterCutoff)
{

}

SpotLight::SpotLight(
    const glm::vec3 direction,
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular,
    const float innerCutoff,
    const float outerCutoff
) : 
	SpotLight(direction, ambient, diffuse, specular, DefaultRange, innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(
    const glm::vec3 direction,
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular,
    const float range,
    const float innerCutoff,
    const float outerCutoff
) :
    Light(LightType::SpotLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
	innerCutoff(innerCutoff),
	outerCutoff(outerCutoff)
{
    attenuationFromRange(range, constant, linear, quadratic);
}

SpotLight::SpotLight(
    const glm::vec3 direction,
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular,
    const float constant,
    const float linear,
    const float quadratic,
    const float innerCutoff,
    const float outerCutoff
) :
    Light(LightType::SpotLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    constant(constant),
    linear(linear),
    quadratic(quadratic),
	innerCutoff(innerCutoff),
	outerCutoff(outerCutoff)
{

}

SpotLight* SpotLight::clone() const
{
    return new SpotLight(direction, ambient, diffuse, specular, constant, linear, quadratic, innerCutoff, outerCutoff);
}

void SpotLight::setRange(const float range)
{
    attenuationFromRange(range, constant, linear, quadratic);
}

}//namespace Renderboi
