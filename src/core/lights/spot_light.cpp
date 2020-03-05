#include "spot_light.hpp"

const std::function<float(float)> SpotLight::linearCoeffFromDesiredRange = [](float range) -> float
{
    return (2.041118599e-7 * range * range) - (7.382441708e-4 * range) + 2.481753022e-1;
};

const std::function<float(float)> SpotLight::quadraticCoeffFromDesiredRange = [](float range) -> float
{
    return (3.682978425e-7 * range * range) - (1.318877109e-3 * range) + 4.032644093e-1;
};

SpotLight::SpotLight(glm::vec3 direction) :
    SpotLight(direction, DEFAULT_SPOT_LIGHT_RANGE)
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
	SpotLight(direction, ambient, diffuse, specular, DEFAULT_SPOT_LIGHT_RANGE, DEFAULT_SPOT_LIGHT_INNER_CUTOFF, DEFAULT_SPOT_LIGHT_OUTER_CUTOFF)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range) :
	SpotLight(direction, ambient, diffuse, specular, range, DEFAULT_SPOT_LIGHT_INNER_CUTOFF, DEFAULT_SPOT_LIGHT_OUTER_CUTOFF)
{

}

SpotLight::SpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float innerCutoff, float outerCutoff) : 
	SpotLight(direction, ambient, diffuse, specular, DEFAULT_SPOT_LIGHT_RANGE, innerCutoff, outerCutoff)
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
    constant = 1.f;
    linear = linearCoeffFromDesiredRange(range);
    quadratic = quadraticCoeffFromDesiredRange(range);
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