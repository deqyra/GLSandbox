/**
    GLTest, perspective_triangle_example.hpp
    Purpose: Define an example class to display some colored triangles in a GL window, with full perspective handling.

    @author François Brachais (deqyra)
    @version 1.0 17/06/2019
 */
#ifndef PERSPECTIVE_TRIANGLE_EXAMPLE_HPP
#define PERSPECTIVE_TRIANGLE_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../gl_example.hpp"
#include "../tools/input_processor.hpp"

class PerspectiveTriangleExample : public GLExample, public InputProcessor
{
    private:
        Shader _shader;

        unsigned int _position_vbo;
        unsigned int _color_vbo;
        unsigned int _ebo;
        unsigned int _vao;

        float _vertices[24];
        float _colors[18];
        unsigned int _indices[8];

        float _angle;
        bool _autoRotate;
        float _speedFactor;
        float _time;

        void setupBuffers();
        void unsetBuffers();

    public:
        PerspectiveTriangleExample();
        virtual ~PerspectiveTriangleExample();
        virtual void run(GLFWwindow* window);

        // Overridden InputProcessor callbacks
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif//PERSPECTIVE_TRIANGLE_EXAMPLE_HPP