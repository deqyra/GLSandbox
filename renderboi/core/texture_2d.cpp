#include "texture_2d.hpp"

#include <stdexcept>

#include <glad/gl.h>
#include <stb_image/stb_image.hpp>

#include "pixel_space.hpp"

#include <renderboi/utilities/to_string.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include <cpptools/exceptions/index_out_of_bounds_error.hpp>

namespace Renderboi
{

using ReLoc = ResourceLocator;
using ReType = ResourceType;

std::unordered_map<unsigned int, unsigned int> Texture2D::_locationRefCounts = std::unordered_map<unsigned int, unsigned int>();
std::unordered_map<std::string , unsigned int> Texture2D::_pathsToIds  = std::unordered_map<std::string, unsigned int>();

Texture2D::Texture2D(const std::string& filename, const PixelSpace space) :
    _path(filename)
{
    auto it = _pathsToIds.find(filename);
    // If the image is already handled by a Texture2D instance...
    if (it != _pathsToIds.end())
    {
        // Just copy the location and increase the refcount
        _location = it->second;
        _locationRefCounts[_location]++;
    }
    // If the image is not being handled by a Texture2D instance...
    else
    {
        // Load the image
        _location = _LoadTextureFromFile(ReLoc::locate(ReType::Texture, filename), space);
        // Map the new texture location to image filename and set a refcount
        _pathsToIds[filename] = _location;
        _locationRefCounts[_location] = 1;
    }
}

Texture2D::Texture2D(const Texture2D& other) :
    _location(other._location),
    _path(other._path)
{
    // The same texture is being handled by one more resource: increase the refcount
    _locationRefCounts[_location]++;
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    // Let go of the content currently in place
    _cleanup();

    // Copy the filename, location, and increase the ref count
    _location = other._location;
    _path = other._path;
    _locationRefCounts[_location]++;

    return *this;
}

Texture2D::~Texture2D()
{
    // Let go of the content currently in place
    _cleanup();
}

void Texture2D::_cleanup()
{
    // Decrease the ref count
    unsigned int count = --_locationRefCounts[_location];
    // If the resource is not used anymore...
    if (!count)
    {
        // Remove ID map
        _pathsToIds.erase(_path);
        // Free the resource on the GPU
        glDeleteTextures(1, &_location);
    };
}

unsigned int Texture2D::_LoadTextureFromFile(const std::string& filename, const PixelSpace space)
{
    // Create a texture resource on the GPU
    unsigned int location;
    glGenTextures(1, &location);

    // Load the image from disk
    int width, height, nChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nChannels == 1)
            format = GL_RED;
        else if (nChannels == 4)
            format = GL_RGBA;

        GLenum internalFormat = GL_RGB;
        if (space == PixelSpace::sRGB)
        {
            internalFormat = GL_SRGB;
            if (format == GL_RGBA)
                internalFormat = GL_SRGB_ALPHA;
        }

        // Send the texture to the GPU
        glBindTexture(GL_TEXTURE_2D, location);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
        std::string s = "Texture2D: failed to load image located at \"" + filename + "\".";
        throw std::runtime_error(s.c_str());
    }

    return location;
}

unsigned int Texture2D::location() const
{
    return _location;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _location);
}

void Texture2D::bind(unsigned int unit) const
{
    unsigned int realUnit = GL_TEXTURE0 + unit;
    if (realUnit > MaxTextureUnit)
    {
        std::string s = "Texture2D: cannot bind to texture unit " + std::to_string(realUnit) + ".";
        throw IndexOutOfBoundsError(s);
    }

    glActiveTexture(realUnit);
    glBindTexture(GL_TEXTURE_2D, _location);
}

}//namespace Renderboi
