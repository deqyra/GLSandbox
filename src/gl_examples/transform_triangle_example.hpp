/**
    GLTest, transform_triangle_example.hpp
    Purpose: Define an example class to display some colored triangles in a GL window, with transformations.

    @author François Brachais (deqyra)
    @version 1.0 15/06/2019
 */
#ifndef TRANSFORM_TRIANGLE_EXAMPLE_HPP
#define TRANSFORM_TRIANGLE_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../gl_example.hpp"
#include "../tools/input_processor.hpp"

class TransformTriangleExample : public GLExample, public InputProcessor
{
    private:
        Shader _shader;

        unsigned int _position_vbo;
        unsigned int _color_vbo;
        unsigned int _ebo;
        unsigned int _vao;

        float _vertices[18];
        float _colors[18];
        unsigned int _indices[8];

        float _angle;
        float _autoRotate;
        float _speedFactor;
        float _time;

        void setupBuffers();
        void unsetBuffers();

    public:
        TransformTriangleExample();
        virtual ~TransformTriangleExample();
        virtual void run(GLFWwindow* window);

        // Overridden InputProcessor callbacks
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif//TRANSFORM_TRIANGLE_EXAMPLE_HPP