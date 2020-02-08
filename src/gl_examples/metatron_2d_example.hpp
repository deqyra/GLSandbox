/**
	GLTest, metatron_2d_example.hpp
	Purpose: Define an example class to display a coloured
	Metatron's cube in a GL window.

	@author François Brachais (deqyra)
	@version 1.0 16/06/2019
 */
#ifndef D2_METATRONS_EXAMPLE_HPP
#define D2_METATRONS_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../gl_example.hpp"

class Metatron2DExample : public GLExample
{
    private:
        Shader _shader;

	    unsigned int _position_vbo;
	    unsigned int _color_vbo;
	    unsigned int _ebo;
	    unsigned int _vao;

	    float _vertices[39];
	    float _colors[39];
	    unsigned int _indices[108];

	    void setupBuffers();
        void unsetBuffers();
        void computeIndices();

    public:
	    Metatron2DExample();
	    virtual ~Metatron2DExample();
	    virtual void run(GLFWwindow* window);
};

#endif//_2D_METATRONS_EXAMPLE_HPP