#include "texture_2d.hpp"

#include "../include/stb_image.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

using std::endl;
using std::runtime_error;
using std::string;
using std::stringstream;

Texture2D::Texture2D(const GLchar* texturePath, const unsigned int textureUnit, bool alphaChannel, bool flipY) :
    _id(0),
    _unit(GL_TEXTURE0 + textureUnit)
{
    if (flipY)
    {
        stbi_set_flip_vertically_on_load(true);
    }

    // Read image.
    unsigned char* data = stbi_load(texturePath, &_width, &_height, &_nChannels, 0);
    if (!data)
    {
        stringstream sstr;
        sstr << "Texture \"" << texturePath << "\" could not be loaded." << endl;
        throw runtime_error(sstr.str().c_str());
    }

    stbi_set_flip_vertically_on_load(false);

    // Generate texture and bind it to texture unit 0 (default).
    glGenTextures(1, &_id);
    glActiveTexture(_unit);
    glBindTexture(GL_TEXTURE_2D, _id);

    // Texture options. Parameters:
    // 1. Which bound texture to set the options of
    // 2. Which option to set on the bound texture
    // 3. Option value

    // Texture wrapping options come per axis: in GL_TEXTURE_WRAP_?, ? can be S, T or R. (S, T, R) are equivalent to the classic (X, Y, Z) but for texels.
    // Available options: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER

    // To set the border color when clamping to border:
    // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // GL_TEXTURE_(MIN|MAG)_FILTER sets the sampling method for upscaled (magnified) and downscaled (minified) textures.
    // Available options:
    // - GL_NEAREST: takes the value of the nearest image pixel.
    // - GL_LINEAR: linear interpolation between closest image pixels.
    // - GL_(NEAREST|LINEAR)_MIPMAP_NEAREST: takes the nearest mipmap for the texture size and uses (nearest neighbour|linear interpolation) for pixel sampling.
    // - GL_(NEAREST|LINEAR)_MIPMAP_LINEAR: linearly interpolates the two closest mipmaps and uses (nearest neighbour|linear interpolation) for pixel sampling.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate texture image on currently bound texture object.
    // Arguments:
    // - Bound texture object to target
    // - Mipmap level to generate a texture image for (0 = base/no mipmap)
    // - Storing format for that texture
    // - Width and height of the texture image
    // - Legacy stuff that should always be 0
    // - Inbound data format
    // - Data type
    // - Data
    unsigned int dataFormat = GL_RGB;
    if (alphaChannel)
    {
        dataFormat = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    // Generate a mipmap for the newly created texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free memory.
    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    glActiveTexture(_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Get texture ID.
unsigned int Texture2D::id()
{
    return _id;
}

// Bind the texture.
void Texture2D::bind()
{
    glActiveTexture(_unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}
