/**
	GLTest, basic_triangle_example.cpp
	Purpose: Implementation of class BasicTriangleExample. See .hpp file.

	@author François Brachais (deqyra)
	@version 1.0 12/06/2019
 */
#include "basic_triangle_example.hpp"
#include "../tools/gl_utils.hpp"

BasicTriangleExample::BasicTriangleExample() :
    _shader("assets/shaders/default.vert", "assets/shaders/vibing_rgb.frag"),
    _position_vbo(0),
    _color_vbo(0),
    _ebo(0),
    _vao(0),
    _vertices{
		-1.0f, -1.0f, 0.0f,
		-0.5f,  0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 0.5f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	},
	_colors{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	},
	_indices{ 0, 1, 2, 3, 4, 1, 3, 5 }
{
}

BasicTriangleExample::~BasicTriangleExample()
{
}

void BasicTriangleExample::setupBuffers()
{
	// Generate a VAO
	glGenVertexArrays(1, &_vao);
    // Once bound, all subsequent VBO and EBO bindings will be stored for proper rebinding
	glBindVertexArray(_vao);

	// Generate a VBO
	glGenBuffers(1, &_position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _position_vbo);
	// Feed vertices to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	// Vertex attribute 0, pointer set-up:
	// - data goes to vertex attribute 0 (position according to our vertex shader)
	// - data comes by packs of 3
	// - data is floating point numbers
	// - data is not normalised
	// - stride is 3 floats wide (0 = tightly packed = [pack size] * [size of type] = 3 in this case)
	// - no offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable vertex attribute 0 (position)
	glEnableVertexAttribArray(0);

	// Generate a VBO
	glGenBuffers(1, &_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
	// Feed colours to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(_colors), _colors, GL_STATIC_DRAW);
	// Set up vertex attribute 1 (colour) pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable vertex attribute 1 (colour)
	glEnableVertexAttribArray(1);

	// Generate an EBO
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	// Feed index sequence to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);

	// Set background to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void BasicTriangleExample::unsetBuffers()
{
    glUseProgram(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_position_vbo);
    _position_vbo = 0;
    glDeleteBuffers(1, &_color_vbo);
    _color_vbo = 0;
    glDeleteBuffers(1, &_ebo);
    _ebo = 0;

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
}

void BasicTriangleExample::run(GLFWwindow* window)
{
    this->setupBuffers();
    
    _shader.use();
    // As the VAO is bound again, the VBO and EBO bindings from setupBuffers() are reinvoked.
	glBindVertexArray(_vao);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
        _shader.setFloat("time", (float)glfwGetTime());
        // glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void*)0);
        glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, (void*)0);
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * 5));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    glfwSetWindowShouldClose(window, false);
    this->unsetBuffers();
}