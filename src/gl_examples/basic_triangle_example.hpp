/**
	GLTest, basic_triangle_example.hpp
	Purpose: Define an example class to display some colored triangles in a GL window.

	@author François Brachais (deqyra)
	@version 1.0 12/06/2019
 */
#ifndef BASIC_TRIANGLE_EXAMPLE_HPP
#define BASIC_TRIANGLE_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../gl_example.hpp"

class BasicTriangleExample: public GLExample
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

        void setupBuffers();
        void unsetBuffers();

	public:
		BasicTriangleExample();
		virtual ~BasicTriangleExample();
		virtual void run(GLFWwindow* window);
};

#endif//BASIC_TRIANGLE_EXAMPLE_HPP