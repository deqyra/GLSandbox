/**
    GLTest, Torus.cpp
    Purpose: Implementation of class Torus. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include <cmath>

#include "torus.hpp"
#include "../../tools/gl_utils.hpp"

#define M_PI 3.14159265358979323846

Torus::Torus() : Torus(DEFAULT_TOROIDAL_RADIUS, DEFAULT_POLOIDAL_RADIUS, DEFAULT_TOROIDAL_VERTEX_RESOLUTION, DEFAULT_POLOIDAL_VERTEX_RESOLUTION)
{

}

Torus::Torus(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes) :
    _toroidalVertexRes(toroidalVertexRes),
    _poloidalVertexRes(poloidalVertexRes),
    _vertices(new float[toroidalVertexRes * poloidalVertexRes * 6]),
    _indices(new unsigned int[poloidalVertexRes * ((toroidalVertexRes * 2) + 2)])
{
    fillVertexData(toroidalRadius, poloidalRadius, toroidalVertexRes, poloidalVertexRes);
}

Torus::~Torus()
{
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

    delete[] _vertices;
    delete[] _indices;
}

void Torus::setupBuffers()
{
	// Generate a VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate a VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Feed vertices to the VBO
    unsigned int vertexDataLength = _toroidalVertexRes * _poloidalVertexRes * 6;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexDataLength, _vertices, GL_STATIC_DRAW);

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
    unsigned int indexDataLength = _poloidalVertexRes * ((_toroidalVertexRes * 2) + 2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexDataLength, _indices, GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}

void Torus::draw()
{
    glBindVertexArray(_vao);
    unsigned int stripSize = (_toroidalVertexRes * 2) + 2;
    for (unsigned int i = 0; i < _poloidalVertexRes; i++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, stripSize, GL_UNSIGNED_INT, (void*)(i * stripSize * sizeof(unsigned int)));
    }
}

void Torus::fillVertexData(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes)
{
    float toroidalAngleStep = (2 * M_PI) / (float)(toroidalVertexRes);
    float poloidalAngleStep = (2 * M_PI) / (float)(poloidalVertexRes);

    // Generate vertex position and colors
    for (unsigned int i = 0; i < poloidalVertexRes; i++)
    {
        float projectedGap = (1 - cos(i * poloidalAngleStep)) * poloidalRadius;
        float innerPeripheralRadius = toroidalRadius - poloidalRadius;
        float ringRadius = innerPeripheralRadius + projectedGap;
        float ringHeight = sin(i * poloidalAngleStep) * poloidalRadius;

        for (unsigned int j = 0; j < toroidalVertexRes; j++)
        {
            float currentAngle = j * toroidalAngleStep;
            float vertexX = cos(currentAngle) * ringRadius;
            float vertexZ = sin(currentAngle) * ringRadius;

            unsigned int index = (i * toroidalVertexRes) + j;
            // Positions
            _vertices[(index * 6)]     = vertexX;
            _vertices[(index * 6) + 1] = ringHeight;
            _vertices[(index * 6) + 2] = vertexZ;
            // Colors
            _vertices[(index * 6) + 3] = 1.f;
            _vertices[(index * 6) + 4] = 1.f;
            _vertices[(index * 6) + 5] = 1.f;
        }
    }

    // Generate index sequences for triangle strips
    unsigned int stripSize = (toroidalVertexRes * 2) + 2;
    for (unsigned int i = 0; i < poloidalVertexRes; i++)
    {
        for (unsigned int j = 0; j < toroidalVertexRes; j++)
        {
            unsigned int currentVertex = (i * toroidalVertexRes) + j;
            unsigned int nextVertex;
            if (i == poloidalVertexRes - 1)
            {
                nextVertex = j;
            }
            else
            {
                nextVertex = ((i + 1) * toroidalVertexRes) + j;
            }

            unsigned int baseIndex  = (i * stripSize) + (j * 2);
            _indices[baseIndex]     = currentVertex;
            _indices[baseIndex + 1] = nextVertex;
        }

        unsigned int currentVertex = i * toroidalVertexRes;
        unsigned int nextVertex;
        if (i == poloidalVertexRes - 1)
        {
            nextVertex = 0;
        }
        else
        {
            nextVertex = (i + 1) * toroidalVertexRes;
        }

        unsigned int baseIndex  = (i * stripSize) + (toroidalVertexRes * 2);
        _indices[baseIndex]     = currentVertex;
        _indices[baseIndex + 1] = nextVertex;
    }
}