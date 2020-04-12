#include "torus_generator.hpp"

#include <vector>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>

#include "../vertex.hpp"

#include "../../tools/gl_utils.hpp"

constexpr double Pi = glm::pi<double>();

TorusGenerator::TorusGenerator() :
    TorusGenerator(DefaultToroidalRadius, DefaultPoloidalRadius, DefaultToroidalVertexResolution, DefaultPoloidalVertexResolution)
{

}

TorusGenerator::TorusGenerator(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes) :
    parameters{ toroidalRadius, poloidalRadius, toroidalVertexRes, poloidalVertexRes }
{

}

TorusGenerator::TorusGenerator(Parameters parameters) :
    parameters(parameters)
{

}

MeshPtr TorusGenerator::generatePtr()
{
    Parameters& p(parameters);

    unsigned int nVertex = (unsigned int)(p.toroidalVertexRes * p.poloidalVertexRes);
    std::vector<Vertex> vertices = std::vector<Vertex>(nVertex);

    unsigned int singleStripLength = (p.toroidalVertexRes * 2) + 2;
    unsigned int stripTotalLength = p.poloidalVertexRes * singleStripLength;
    std::vector<unsigned int> indices = std::vector<unsigned int>(stripTotalLength);

    float toroidalAngleStep = (float)(2 * Pi) / (float)(p.toroidalVertexRes);
    float poloidalAngleStep = (float)(2 * Pi) / (float)(p.poloidalVertexRes);

    // Generate vertex position, colors and normals
    for (unsigned int i = 0; i < p.poloidalVertexRes; i++)
    {
        float pAngle = i * poloidalAngleStep;
        float projectedGap = (1 - cos(pAngle)) * p.poloidalRadius;
        float innerPeripheralRadius = p.toroidalRadius - p.poloidalRadius;
        float ringRadius = innerPeripheralRadius + projectedGap;
        float ringHeight = sin(pAngle) * p.poloidalRadius;

        for (unsigned int j = 0; j < p.toroidalVertexRes; j++)
        {
            float tAngle = j * toroidalAngleStep;

            float vertexX = cos(tAngle) * ringRadius;
            float vertexY = ringHeight;
            float vertexZ = sin(tAngle) * ringRadius;

            float normalX = -cos(pAngle) * cos(tAngle);
            float normalY = sin(pAngle);
            float normalZ = -cos(pAngle) * sin(tAngle);

            glm::vec3 position = glm::vec3(vertexX, vertexY, vertexZ);
            glm::vec3 color    = glm::vec3(WHITE);
            glm::vec3 normal   = glm::vec3(normalX, normalY, normalZ);
            glm::vec2 texCoord = glm::vec2(0.f);

            unsigned int index = (i * p.toroidalVertexRes) + j;
            // Positions
            vertices[index].position = position;
            vertices[index].color    = color;
            vertices[index].normal   = normal;
            vertices[index].texCoord = texCoord;
        }
    }

    // Generate index sequences for triangle strips
    unsigned int stripSize = (p.toroidalVertexRes * 2) + 2;
    for (unsigned int i = 0; i < p.poloidalVertexRes; i++)
    {
        for (unsigned int j = 0; j < p.toroidalVertexRes; j++)
        {
            unsigned int currentVertex = (i * p.toroidalVertexRes) + j;
            unsigned int nextVertex;
            if (i == p.poloidalVertexRes - 1)
            {
                nextVertex = j;
            }
            else
            {
                nextVertex = ((i + 1) * p.toroidalVertexRes) + j;
            }

            unsigned int index  = (i * stripSize) + (j * 2);
            indices[index]     = currentVertex;
            indices[index + 1] = nextVertex;
        }

        unsigned int currentVertex = i * p.toroidalVertexRes;
        unsigned int nextVertex;
        if (i == p.poloidalVertexRes - 1)
        {
            nextVertex = 0;
        }
        else
        {
            nextVertex = (i + 1) * p.toroidalVertexRes;
        }

        unsigned int index  = (i * stripSize) + (p.toroidalVertexRes * 2);
        indices[index]     = currentVertex;
        indices[index + 1] = nextVertex;
    }

    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLE_STRIP);
}
