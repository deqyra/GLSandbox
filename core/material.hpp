#ifndef CORE__MATERIAL_HPP
#define CORE__MATERIAL_HPP

#include <glm/glm.hpp>
#include <vector>

#include "texture_2d.hpp"

// Material with fixed ambient, diffuse and specular components (and shininess), as well as diffuse and specular maps
class Material
{
    private:
        std::vector<Texture2D> _diffuseMaps;
        std::vector<Texture2D> _specularMaps;

    public:
        static constexpr unsigned int DiffuseMapMaxCount = 8;
        static constexpr unsigned int SpecularMapMaxCount = 8;

        Material();
        Material(const Material& other);
        Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

        std::vector<Texture2D> getDiffuseMaps();
        std::vector<Texture2D> getSpecularMaps();

        unsigned int pushDiffuseMap(Texture2D tex);
        unsigned int pushSpecularMap(Texture2D tex);
        unsigned int popDiffuseMap();
        unsigned int popSpecularMap();
        
        unsigned int setDiffuseMaps(std::vector<Texture2D> diffuseMaps);
        unsigned int setSpecularMaps(std::vector<Texture2D> specularMaps);
        unsigned int getDiffuseMapCount();
        unsigned int getSpecularMapCount();

        void bindTextures();

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
};

#endif//CORE__MATERIAL_HPP