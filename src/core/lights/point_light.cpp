#include "point_light.hpp"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    PositionedObject(),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{

}