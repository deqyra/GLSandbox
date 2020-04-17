#include "tetrahedron_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../vertex.hpp"

#include "../gl_macros.hpp"

TetrahedronGenerator::TetrahedronGenerator() :
    TetrahedronGenerator(DefaultSize)
{

}

TetrahedronGenerator::TetrahedronGenerator(float size) :
    parameters{ size, glm::vec3(0.f), false }
{
    
}

TetrahedronGenerator::TetrahedronGenerator(float size, glm::vec3 color) :
    parameters{ size, color, true}
{

}

TetrahedronGenerator::TetrahedronGenerator(float size, float r, float g, float b) :
    parameters{ size, glm::vec3(r, g, b), true }
{
    
}

TetrahedronGenerator::TetrahedronGenerator(Parameters parameters) :
    parameters(parameters)
{
    
}

MeshPtr TetrahedronGenerator::generatePtr()
{
    const unsigned int nVertices = 12;

    glm::vec3 top           = glm::vec3( 0.f,               0.75f * sqrt(2.f),  0.f)                * parameters.size;
    glm::vec3 baseBackLeft  = glm::vec3(-0.5f * sqrt(3.f), -0.25f * sqrt(2.f), -0.5f)               * parameters.size;
    glm::vec3 baseBackRight = glm::vec3( 0.5f * sqrt(3.f), -0.25f * sqrt(2.f), -0.5f)               * parameters.size;
    glm::vec3 baseFront     = glm::vec3( 0.f,              -0.25f * sqrt(2.f),  1.f)                * parameters.size;

    std::vector<Vertex> vertices = {
        // Position         // Color            // Normal       // Tex coord
        { baseBackLeft,     glm::vec3(RED),     -top,           glm::vec2(0.f,  0.f) },     // Vertex 1     // Face 1
        { baseBackRight,    glm::vec3(GREEN),   -top,           glm::vec2(1.f,  0.f) },     // Vertex 2     // -Y
        { baseFront,        glm::vec3(BLUE),    -top,           glm::vec2(0.5f, 1.f) },     // ...

        { baseBackRight,    glm::vec3(GREEN),   -baseFront,     glm::vec2(0.f,  0.f) },                     // Face 2
        { baseBackLeft,     glm::vec3(RED),     -baseFront,     glm::vec2(1.f,  0.f) },                     // -Z
        { top,              glm::vec3(WHITE),   -baseFront,     glm::vec2(0.5f, 1.f) },

        { top,              glm::vec3(WHITE),   -baseBackRight, glm::vec2(0.5f, 1.f) },                     // Face 3
        { baseBackLeft,     glm::vec3(RED),     -baseBackRight, glm::vec2(0.f,  0.f) },                     // -X
        { baseFront,        glm::vec3(BLUE),    -baseBackRight, glm::vec2(0.f,  1.f) },

        { baseFront,        glm::vec3(BLUE),    -baseBackLeft,  glm::vec2(0.f,  0.f) },                     // Face 4
        { baseBackRight,    glm::vec3(GREEN),   -baseBackLeft,  glm::vec2(1.f,  0.f) },                     //  X
        { top,              glm::vec3(WHITE),   -baseBackLeft,  glm::vec2(0.5f, 1.f) }
    };

    if (parameters.useColor)
    {
        for (int i = 0; i < nVertices; i++)
        {
            vertices[i].color = parameters.color;
        }
    }

    std::vector<unsigned int> indices = {
        0,  1,  2,
        3,  4,  5,
        6,  7,  8,
        9, 10, 11
    };

    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}