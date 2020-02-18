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

Torus::Torus(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes, bool drawNormals) :
    _toroidalVertexRes(toroidalVertexRes),
    _poloidalVertexRes(poloidalVertexRes),
    _drawNormals(drawNormals)
{
    unsigned int nVertex = (unsigned int)(toroidalVertexRes * poloidalVertexRes);
    _vertexDataLength = drawNormals ? (nVertex * DATA_STRIDE * 3)
                                    : (nVertex * DATA_STRIDE);
    _vertices = new float[_vertexDataLength];

    unsigned int meshStrips = (unsigned int)(poloidalVertexRes * ((toroidalVertexRes * 2) + 2));
    unsigned int normalLines = nVertex * 2;

    _indexDataLength = drawNormals ? (meshStrips + normalLines)
                                   : meshStrips;
    _indices = new unsigned int[_indexDataLength];

    fillVertexData(toroidalRadius, poloidalRadius, toroidalVertexRes, poloidalVertexRes, drawNormals);
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
    if (_buffersSetUp) return;

    // Generate a VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate a VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Feed vertices to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexDataLength, _vertices, GL_STATIC_DRAW);

    // Specify vertex attribute layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DATA_STRIDE * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, DATA_STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, DATA_STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
	// Enable vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Generate an EBO
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	// Feed index sequence to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indexDataLength, _indices, GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);

    _buffersSetUp = true;
}

void Torus::draw()
{
    glBindVertexArray(_vao);
    unsigned int stripSize = (_toroidalVertexRes * 2) + 2;
    for (unsigned int i = 0; i < _poloidalVertexRes; i++)
    {
        unsigned int offset = i * stripSize;
        glDrawElements(GL_TRIANGLE_STRIP, stripSize, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
    }

    if (_drawNormals)
    {
        unsigned int nLineIndices = _toroidalVertexRes * _poloidalVertexRes * 2;
        unsigned int offset = _poloidalVertexRes * stripSize;
        glDrawElements(GL_LINES, nLineIndices, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
    }
}

void Torus::fillVertexData(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes, bool drawNormals)
{
    float toroidalAngleStep = (float)(2 * M_PI) / (float)(toroidalVertexRes);
    float poloidalAngleStep = (float)(2 * M_PI) / (float)(poloidalVertexRes);

    // Generate vertex position, colors and normals
    for (unsigned int i = 0; i < poloidalVertexRes; i++)
    {
        float pAngle = i * poloidalAngleStep;
        float projectedGap = (1 - cos(pAngle)) * poloidalRadius;
        float innerPeripheralRadius = toroidalRadius - poloidalRadius;
        float ringRadius = innerPeripheralRadius + projectedGap;
        float ringHeight = sin(pAngle) * poloidalRadius;

        for (unsigned int j = 0; j < toroidalVertexRes; j++)
        {
            float tAngle = j * toroidalAngleStep;

            float vertexX = cos(tAngle) * ringRadius;
            float vertexY = ringHeight;
            float vertexZ = sin(tAngle) * ringRadius;

            float normalX = -cos(pAngle) * cos(tAngle);
            float normalY = sin(pAngle);
            float normalZ = -cos(pAngle) * sin(tAngle);

            unsigned int index = (i * toroidalVertexRes) + j;
            index *= DATA_STRIDE;
            // Positions
            _vertices[index]     = vertexX;
            _vertices[index + 1] = vertexY;
            _vertices[index + 2] = vertexZ;
            // Colors
            _vertices[index + 3] = 1.f;
            _vertices[index + 4] = 1.f;
            _vertices[index + 5] = 1.f;
            // Normals
            _vertices[index + 6] = normalX;
            _vertices[index + 7] = normalY;
            _vertices[index + 8] = normalZ;

            if (drawNormals)
            {
                unsigned int index = (i * toroidalVertexRes) + j;
                index = (poloidalVertexRes * toroidalVertexRes) + (index * 2);
                index *= DATA_STRIDE;

                float normalFactor = 0.8f;

                // Base vertex
                _vertices[index]      = vertexX;
                _vertices[index +  1] = vertexY;
                _vertices[index +  2] = vertexZ;
                _vertices[index +  3] = 1.f;    // WH
                _vertices[index +  4] = 1.f;    // IT
                _vertices[index +  5] = 1.f;    // E
                _vertices[index +  6] = normalX;
                _vertices[index +  7] = normalY;
                _vertices[index +  8] = normalZ;
                // Normal vertex
                _vertices[index +  9] = vertexX + (normalX * normalFactor);
                _vertices[index + 10] = vertexY + (normalY * normalFactor);
                _vertices[index + 11] = vertexZ + (normalZ * normalFactor);
                _vertices[index + 12] = 1.f;    // YE
                _vertices[index + 13] = 1.f;    // LL
                _vertices[index + 14] = 0.f;    // OW
                _vertices[index + 15] = 0.f;
                _vertices[index + 16] = 0.f;
                _vertices[index + 17] = 0.f;
            }
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

            unsigned int index  = (i * stripSize) + (j * 2);
            _indices[index]     = currentVertex;
            _indices[index + 1] = nextVertex;
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

        unsigned int index  = (i * stripSize) + (toroidalVertexRes * 2);
        _indices[index]     = currentVertex;
        _indices[index + 1] = nextVertex;
    }
    
    if (drawNormals)
    {
        unsigned int vertexOffset = poloidalVertexRes * toroidalVertexRes;
        unsigned int indexOffset  = poloidalVertexRes * stripSize;
        for (unsigned int i = 0; i < poloidalVertexRes * toroidalVertexRes * 2; i++)
        {
            _indices[indexOffset + i] = vertexOffset + i;
        }
    }
}
