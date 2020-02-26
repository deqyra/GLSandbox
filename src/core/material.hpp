#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>
#include <vector>

#include "texture_2d.hpp"

#define DIFFUSE_MAP_MAX_COUNT   8
#define SPECULAR_MAP_MAX_COUNT  8

class Material
{
    public:
        Material();
        Material(glm::vec3 ambient,
                 glm::vec3 diffuse,
                 glm::vec3 specular,
                 float shininess,
                 std::vector<Texture2D> diffuseMaps = std::vector<Texture2D>(),
                 std::vector<Texture2D> specularMaps = std::vector<Texture2D>()
        );

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        std::vector<Texture2D> diffuseMaps;
        std::vector<Texture2D> specularMaps;
};

#endif//MATERIAL_HPP