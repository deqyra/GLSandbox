#include "directional_light.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction) :
    DirectionalLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f))
{

}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(LightType::DirectionalLight),
    direction(direction),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{

}