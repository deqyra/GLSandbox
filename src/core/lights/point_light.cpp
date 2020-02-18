#include "point_light.hpp"

PointLight::PointLight() :
    PositionedObject(),
    Light(LightType::PointLight),
    ambient(glm::vec3(1.f)),
    diffuse(glm::vec3(1.f)),
    specular(glm::vec3(1.f))
{

}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    PositionedObject(),
    Light(LightType::PointLight),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{

}