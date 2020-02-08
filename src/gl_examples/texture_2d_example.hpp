/**
    GLTest, texture_2d_example.hpp
    Purpose: Define an example class to display
    textured stuff in a GL window.

    @author François Brachais (deqyra)
    @version 1.0 13/09/2019
 */
#ifndef TEXTURE_EXAMPLE_HPP
#define TEXTURE_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../core/texture_2d.hpp"
#include "../gl_example.hpp"
#include "../tools/input_processor.hpp"

class Texture2DExample : public GLExample, public InputProcessor
{
    private:
        Shader _shader;
        Texture2D _texture;
        Texture2D _overlay;

        unsigned int _vbo;
        unsigned int _ebo;
        unsigned int _vao;

        float _vertices[32];
        unsigned int _indices[8];

        float _mixValue;

        void setupBuffers();
        void unsetBuffers();

        // Overriden GLFW callbacks
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:
        Texture2DExample();
        virtual ~Texture2DExample();
        virtual void run(GLFWwindow* window);
};

#endif//TEXTURE_EXAMPLE_HPP