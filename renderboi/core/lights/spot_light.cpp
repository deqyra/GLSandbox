#include "spot_light.hpp"
#include "tools.hpp"

SpotLight::SpotLight(glm::vec3 direction) :
    SpotLight(direction, DefaultRange)
{

}

SpotLight::SpotLight(glm::vec3 direction, float range) :
    SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range)
{

}

SpotLight::SpotLight(glm::vec3 direction, float innerCutoff, float outerCutoff) :
	SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(glm::vec3 direction, float range, float innerCutoff, float outerCutoff) :
	SpotLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), range, innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	SpotLight(direction, ambient, diffuse, specular, DefaultRange, DefaultInnerCutoff, DefaultOuterCutoff)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range) :
	SpotLight(direction, ambient, diffuse, specular, range, DefaultInnerCutoff, DefaultOuterCutoff)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float innerCutoff, float outerCutoff) : 
	SpotLight(direction, ambient, diffuse, specular, DefaultRange, innerCutoff, outerCutoff)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range, float innerCutoff, float outerCutoff) :
    Light(LightType::SpotLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
	innerCutoff(innerCutoff),
	outerCutoff(outerCutoff)
{
    attenuationFromRange(range, constant, linear, quadratic);
}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutoff, float outerCutoff) :
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

SpotLight* SpotLight::clone()
{
    return new SpotLight(direction, ambient, diffuse, specular, constant, linear, quadratic, innerCutoff, outerCutoff);
}

void SpotLight::setRange(float range)
{
    attenuationFromRange(range, constant, linear, quadratic);
}
