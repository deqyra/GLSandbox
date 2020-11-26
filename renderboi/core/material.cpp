#include "material.hpp"

#include <string>

#include <cpptools/exceptions/index_out_of_bounds_error.hpp>
#include <cpptools/exceptions/unmatched_array_size_error.hpp>

Material::Material(const Material& other) :
    ambient(other.ambient),
    diffuse(other.diffuse),
    specular(other.specular),
    shininess(other.shininess),
    _diffuseMaps(other._diffuseMaps),
    _specularMaps(other._specularMaps)
{

}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    shininess(shininess),
    _diffuseMaps(),
    _specularMaps()
{
    _diffuseMaps.reserve(DiffuseMapMaxCount);
    _specularMaps.reserve(SpecularMapMaxCount);
}

std::vector<Texture2D> Material::getDiffuseMaps()
{
    return _diffuseMaps;
}

std::vector<Texture2D> Material::getSpecularMaps()
{
    return _specularMaps;
}

unsigned int Material::pushDiffuseMap(Texture2D tex)
{
    unsigned int size = (unsigned int) _diffuseMaps.size();
    if (size >= DiffuseMapMaxCount)
    {
        std::string s = "Material: cannot push diffuse map as array is full.";
        throw IndexOutOfBoundsError(s);
    }

    _diffuseMaps.push_back(tex);
    return size + 1;
}

unsigned int Material::pushSpecularMap(Texture2D tex)
{
    unsigned int size = (unsigned int) _specularMaps.size();
    if (size >= SpecularMapMaxCount)
    {
        std::string s = "Material: cannot push specular map as array is full.";
        throw IndexOutOfBoundsError(s);
    }

    _specularMaps.push_back(tex);
    return size + 1;
}

unsigned int Material::popDiffuseMap()
{
    unsigned int size = (unsigned int) _diffuseMaps.size();
    if (size == 0)
    {
        std::string s = "Material: cannot pop diffuse map as array is empty.";
        throw IndexOutOfBoundsError(s);
    }

    _diffuseMaps.pop_back();
    return size - 1;
}

unsigned int Material::popSpecularMap()
{
    unsigned int size = (unsigned int) _specularMaps.size();
    if (size == 0)
    {
        std::string s = "Material: cannot pop specular map as array is empty.";
        throw IndexOutOfBoundsError(s);
    }

    _specularMaps.pop_back();
    return size - 1;
}

unsigned int Material::setDiffuseMaps(std::vector<Texture2D> diffuseMaps)
{
    unsigned int size = (unsigned int) diffuseMaps.size();
    if (size > DiffuseMapMaxCount)
    {
        std::string s = "Material: cannot set diffuse maps to array of size " + std::to_string(_diffuseMaps.size()) + ".";
        throw UnmatchedArraySizeError(s);
    }

    _diffuseMaps = diffuseMaps;
    return size;
}

unsigned Material::setSpecularMaps(std::vector<Texture2D> specularMaps)
{
    unsigned int size = (unsigned int) specularMaps.size();
    if (specularMaps.size() >= SpecularMapMaxCount)
    {
        std::string s = "Material: cannot set specular maps to array of size " + std::to_string(_specularMaps.size()) + ".";
        throw UnmatchedArraySizeError(s);
    }

    _specularMaps = specularMaps;
    return size;
}

unsigned int Material::getDiffuseMapCount()
{
    return (unsigned int) _diffuseMaps.size();
}

unsigned int Material::getSpecularMapCount()
{
    return (unsigned int) _specularMaps.size();
}

void Material::bindTextures()
{
    for (unsigned int i = 0; i < _diffuseMaps.size(); i++)
    {
        _diffuseMaps[i].bind(i);
    }

    for (unsigned int i = 0; i < _diffuseMaps.size(); i++)
    {
        _diffuseMaps[i].bind(DiffuseMapMaxCount + i);
    }
}
