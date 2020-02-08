/**
    GLTest, texture.hpp
    Purpose: Texture2D handler. Reads image files and generates textures.

    @author François Brachais (deqyra)
    @version 1.0 16/09/2019
 */

#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/glad.h>

class Texture2D
{
    private:
        unsigned int _id;
        unsigned int _unit;

        int _width;
        int _height;
        int _nChannels;

    public:
        Texture2D(const GLchar* texturePath, const unsigned int textureUnit = 0, bool alphaChannel = false, bool flipY = false);
        ~Texture2D();

        // Get texture ID.
        unsigned int id();

        // Bind the texture.
        void bind();
};

#endif//TEXTURE_2D_HPP