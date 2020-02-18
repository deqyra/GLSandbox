#include "material.hpp"

Material::Material() :
    ambient(glm::vec3(0.15f)),
    diffuse(glm::vec3(0.6f)),
    specular(glm::vec3(1.f)),
    shininess(12.8f)
{

}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    shininess(shininess)
{

}