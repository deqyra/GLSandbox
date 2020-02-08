/**
    GLTest, cube.cpp
    Purpose: Implementation of class Cube. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include "cube.hpp"
#include "../../tools/gl_utils.hpp"

Cube::Cube(float size) : Mesh(),
    _vertices{
        // Position                             // Colours
         size / 2.f,  size / 2.f,  size / 2.f,  WHITE,
         size / 2.f,  size / 2.f, -size / 2.f,  RED,
         size / 2.f, -size / 2.f,  size / 2.f,  GREEN,
         size / 2.f, -size / 2.f, -size / 2.f,  BLUE,
        -size / 2.f,  size / 2.f,  size / 2.f,  YELLOW,
        -size / 2.f,  size / 2.f, -size / 2.f,  MAGENTA,
        -size / 2.f, -size / 2.f,  size / 2.f,  CYAN,
        -size / 2.f, -size / 2.f, -size / 2.f,  BLACK
    },
    _indices{
        0, 1, 2, 3, 6, 7, 4, 5, 0, 1,   // Side face strip
        4, 6, 0, 2,                     // Top face strip
        1, 3, 5, 7                      // Bottom face strip
    }
{
    
}

Cube::~Cube()
{
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void Cube::setupBuffers()
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

void Cube::draw()
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, (void*) 0);
    glDrawElements(GL_TRIANGLE_STRIP,  4, GL_UNSIGNED_INT, (void*)(10 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_STRIP,  4, GL_UNSIGNED_INT, (void*)(14 * sizeof(unsigned int)));
}