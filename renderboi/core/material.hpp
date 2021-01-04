#ifndef RENDERBOI__CORE__MATERIAL_HPP
#define RENDERBOI__CORE__MATERIAL_HPP

#include <glm/glm.hpp>
#include <vector>

#include "texture_2d.hpp"

namespace Renderboi
{

/// @brief Material with ambient, diffuse and specular components, as well as 
/// diffuse and specular maps.
class Material
{
private:
    /// @brief Array of textures for the diffuse reflection of the material.
    std::vector<Texture2D> _diffuseMaps;
    /// @brief Array of textures for the specular reflection of the material.
    std::vector<Texture2D> _specularMaps;

public:
    /// @brief The maximum amount of diffuse maps the material can have.
    static constexpr unsigned int DiffuseMapMaxCount = 8;

    /// @brief The maximum amount of specular maps the material can have.
    static constexpr unsigned int SpecularMapMaxCount = 8;

    /// @brief The fixed ambient reflection component of the material.
    glm::vec3 ambient;

    /// @brief The fixed diffuse reflection component of the material.
    glm::vec3 diffuse;

    /// @brief The fixed specular reflection component of the material.
    glm::vec3 specular;

    /// @brief The shininess (specular strength) of the material.
    float shininess;

    Material(const Material& other);
    
    /// @param ambient Fixed ambient component of the reflection of the material.
    /// @param diffuse Fixed diffuse component of the reflection of the material.
    /// @param specular Fixed specular component of the reflection of the material.
    /// @param shininess Shininess (specular strength) of the material.
    Material(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const float shininess);

    /// @brief Get a copy of the array of diffuse maps of the material.
    ///
    /// @return The diffuse maps of the material in an array.
    std::vector<Texture2D> getDiffuseMaps() const;

    /// @brief Get a copy of the array of specular maps of the material.
    ///
    /// @return The specular maps of the material in an array.
    std::vector<Texture2D> getSpecularMaps() const;

    /// @brief Add a new texture to the array of diffuse maps of the
    /// material. The new map will be added behind other diffuse maps.
    ///
    /// @param tex The new texture to be added.
    ///
    /// @return The amount of diffuse maps the material has, after adding. 
    ///
    /// @exception If this method is called when the maximum count of 
    /// diffuse maps was already reached for this material, it will throw
    /// an IndexOutOfBoundsError.
    unsigned int pushDiffuseMap(const Texture2D tex);

    /// @brief Add a new texture to the array of specular maps of the
    /// material. The new map will be added behind other specular maps.
    ///
    /// @param tex The new texture to be added.
    ///
    /// @return The amount of specular maps the material has, after adding.
    ///
    /// @exception If this method is called when the maximum count of 
    /// specular maps was already reached for this material, it will throw
    /// an IndexOutOfBoundsError.
    unsigned int pushSpecularMap(const Texture2D tex);

    /// @brief Remove the last texture in the array of diffuse maps of the
    /// material.
    ///
    /// @return The amount of diffuse maps the material has, after removing.
    ///
    /// @exception If this method is called when the material has no
    /// diffuse maps, it will throw an IndexOutOfBoundsError.
    unsigned int popDiffuseMap();

    /// @brief Remove the last texture in the array of specular maps of the
    /// material.
    ///
    /// @return The amount of specular maps the material has, after removing.
    ///
    /// @exception If this method is called when the material has no
    /// specular maps, it will throw an IndexOutOfBoundsError.
    unsigned int popSpecularMap();
    
    /// @brief Replace the whole array of diffuse maps of the material with
    /// a new one.
    ///
    /// @param diffuseMaps The new array of diffuse maps. Its size cannot 
    /// exceed Material::DiffuseMapMaxCount.
    ///
    /// @return The amount of diffuse maps the material has, after replacing.
    ///
    /// @exception If the size of the provided array exceeds 
    /// Material::DiffuseMapMaxCount, this method will throw an 
    /// UnmatchedArraySizeError.
    unsigned int setDiffuseMaps(const std::vector<Texture2D> diffuseMaps);
    
    /// @brief Replace the whole array of specular maps of the material with
    /// a new one.
    ///
    /// @param specularMaps The new array of specular maps. Its size cannot 
    /// exceed Material::SpecularMapMaxCount.
    ///
    /// @return The amount of specular maps the material has, after replacing.
    ///
    /// @exception If the size of the provided array exceeds 
    /// Material::SpecularMapMaxCount, this method will throw an 
    /// UnmatchedArraySizeError.
    unsigned int setSpecularMaps(const std::vector<Texture2D> specularMaps);

    /// @brief Get the count of diffuse maps the material currently has.
    ///
    /// @return The count of diffuse maps the material currently has.
    unsigned int getDiffuseMapCount() const;


    /// @brief Get the count of specular maps the material currently has.
    ///
    /// @return The count of specular maps the material currently has.
    unsigned int getSpecularMapCount() const;

    /// @brief Bind all textures registered in the material to a texture
    /// channel on the GPU.
    void bindTextures() const;
};

}//namespace Renderboi

#endif//RENDERBOI__CORE__MATERIAL_HPP