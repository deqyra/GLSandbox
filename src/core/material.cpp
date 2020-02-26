#include "material.hpp"

Material::Material() :
    Material(glm::vec3(0.15f), glm::vec3(0.6f), glm::vec3(1.f), 12.8f)
{

}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::vector<Texture2D> diffuseMaps, std::vector<Texture2D> specularMaps) :
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    shininess(shininess),
    diffuseMaps(diffuseMaps),
    specularMaps(specularMaps)
{

}