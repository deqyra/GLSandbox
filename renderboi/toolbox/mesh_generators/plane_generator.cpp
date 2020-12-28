#include "plane_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/vertex.hpp>

#include "../common_macros.hpp"

PlaneGenerator::PlaneGenerator() :
    parameters{
        1.f,                // tileSizeX
        1.f,                // tileSizeY
        1,                  // tileAmountX
        1,                  // tileAmountY
        1.f,                // xTexSize
        1.f,                // yTexSize
        0.f,                // xTexCoordOffset
        0.f,                // yTexCoordOffset
        false,              // invertXTexCoords
        false,              // invertYTexCoords
        0.f,                // texRotation
        {1.f, 1.f, 1.f}     // color
    }
{

}

PlaneGenerator::PlaneGenerator(Parameters parameters) :
    parameters(parameters)
{
    if (parameters.xTexSize <= 0.f)
        this->parameters.xTexSize = parameters.tileAmountX * parameters.tileSizeX;

    if (parameters.yTexSize <= 0.f)
        this->parameters.yTexSize = parameters.tileAmountY * parameters.tileSizeY;
}

MeshPtr PlaneGenerator::generatePtr() const
{
    const Parameters& p(parameters);
    const unsigned int nVertices = (p.tileAmountX + 1) * (p.tileAmountY + 1);

    std::vector<Vertex> vertices;
    vertices.reserve(nVertices);

    for (unsigned int j = 0; j < p.tileAmountY + 1; j++)
    {
        float yPos = j * p.tileSizeY;

        float baseTexY = yPos / p.yTexSize;
        if (p.invertYTexCoords) baseTexY = p.yTexSize - baseTexY;

        for (unsigned int i = 0; i < p.tileAmountX + 1; i++)
        {
            float xPos = i * p.tileSizeX;

            float baseTexX = xPos / p.xTexSize;
            if (p.invertXTexCoords) baseTexX = p.xTexSize - baseTexX;

            // Do a complex rotation to find the rotated texture coordinates.
            // The opposite rotation is actually calculated, because the current 
            // vertex coordinates are not being rotated. What happens then is 
            // that the texture coordinates are moving in reverse relative to 
            // the vertex coordinates.

            // Complex coefficients
            float rotationReal = glm::cos(-p.texRotation);
            float rotationImag = glm::sin(-p.texRotation);

            // Actual rotation
            float rotatedTexX = (baseTexX * rotationReal) - (baseTexY * rotationImag);
            float rotatedTexY = (baseTexX * rotationImag) + (baseTexY * rotationReal);

            // Add offset
            float fullTexX = rotatedTexX - p.xTexCoordOffset;
            float fullTexY = rotatedTexY - p.yTexCoordOffset;

            // Build vertex
            Vertex vertex = {
                {xPos, yPos, 0.f},      // Position
                p.color,                // Color
                {0.f, 0.f, 1.f},        // Normal
                {fullTexX, fullTexY}    // Tex coord
            };
            vertices.push_back(vertex);
        } 
    }

    const unsigned int nIndices = ((p.tileAmountX + 1) * (p.tileAmountY) * 2);

    std::vector<unsigned int> indices;
    indices.reserve(nIndices);

    unsigned int xVertexAmount = p.tileAmountX + 1;

    for (unsigned int j = 0; j < p.tileAmountY; j++)
    {
        for (unsigned int i = 0; i < xVertexAmount; i++)
        {
            indices.push_back(i + (xVertexAmount * (j + 1)));
            indices.push_back(i + (xVertexAmount * j));
        }
    }

    unsigned int primitiveSize = 2 * xVertexAmount;
    std::vector<unsigned int> primitiveSizes(p.tileAmountY, primitiveSize);

    std::vector<void*> primitiveOffsets(p.tileAmountY);
    for (unsigned int j = 0; j < p.tileAmountY; j++)
    {
        primitiveOffsets[j] = (void*)(j * primitiveSize * sizeof(int));
    }

    return std::make_shared<Mesh>(GL_TRIANGLE_STRIP, vertices, indices, primitiveSizes, primitiveOffsets);
}