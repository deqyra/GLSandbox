/**
    GLTest, axes.cpp
    Purpose: Implementation of class Axes. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include "axes.hpp"
#include "../../tools/gl_utils.hpp"

Axes::Axes(float size) : Mesh(),
    _vertices{
        // Position        // Colours
        0.f,  0.f,  0.f,   RED,
        size, 0.f,  0.f,   RED,
        0.f,  0.f,  0.f,   GREEN,
        0.f,  size, 0.f,   GREEN,
        0.f,  0.f,  0.f,   BLUE,
        0.f,  0.f,  size,  BLUE
    },
    _indices{
        0, 1,   // X axis
        2, 3,   // Y axis
        4, 5    // Z axis
    }
{
    
}

Axes::~Axes()
{
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void Axes::setupBuffers()
{
	// Generate a VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate a VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Feed vertices to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // Specify vertex attribute layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable vertex attributes
	glEnableVertexAttribArray(0);
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

void Axes::draw()
{
    glBindVertexArray(_vao);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*) 0);
}