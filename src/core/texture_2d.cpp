#include "texture_2d.hpp"

#include "../../include/glad/glad.h"
#include "../../include/stb_image.hpp"

#include <stdexcept>
#include "../tools/exceptions/index_out_of_bounds_error.hpp"

std::unordered_map<unsigned int, unsigned int> Texture2D::_refCount = std::unordered_map<unsigned int, unsigned int>();
std::unordered_map<std::string , unsigned int> Texture2D::_pathIds  = std::unordered_map<std::string, unsigned int>();

Texture2D::Texture2D(std::string path) :
    _path(path)
{
    auto it = _pathIds.find(path);
    if (it != _pathIds.end())
    {
        _id = it->second;
        _refCount[_id]++;
    }
    else
    {
        _id = loadTextureFromFile(path);
        _pathIds[path] = _id;
        _refCount[_id] = 1;
    }
}

Texture2D::Texture2D(const Texture2D& other) :
    _id(other._id),
    _path(other._path)
{
    _refCount[_id]++;
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    cleanup();

    _id = other._id;
    _path = other._path;
    _refCount[_id]++;

    return *this;
}

Texture2D::~Texture2D()
{
    cleanup();
}

void Texture2D::cleanup()
{
    // Decrease the ref count
    unsigned int count = --_refCount[_id];
    // If empty, destroy resource
    if (!count)
    {
        _pathIds.erase(_path);
        glDeleteTextures(1, &_id);
    };
}

unsigned int Texture2D::loadTextureFromFile(const std::string path)
{
    unsigned int name;
    glGenTextures(1, &name);

    int width, height, nChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nChannels == 1)
            format = GL_RED;
        else if (nChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, name);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
        std::string s = "Texture2D: failed to load path " + path;
        throw std::runtime_error(s.c_str());
    }

    return name;
}

// Get texture ID.
unsigned int Texture2D::id()
{
    return _id;
}

// Bind the texture.
void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::bind(unsigned int unit)
{
    unsigned int realUnit = GL_TEXTURE0 + unit;
    if (realUnit)
    {
        std::string s = "Texture2D: cannot bind to texture unit " + std::to_string(realUnit) + ".";
        throw IndexOutOfBoundsError(s);
    }

    glActiveTexture(realUnit);
    glBindTexture(GL_TEXTURE_2D, _id);
}
