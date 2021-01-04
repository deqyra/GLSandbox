#include "cube_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/vertex.hpp>

#include "../common_macros.hpp"

namespace Renderboi
{

CubeGenerator::CubeGenerator() :
    CubeGenerator(DefaultSize)
{

}

CubeGenerator::CubeGenerator(float size) :
    parameters{ size, glm::vec3(0.f), false }
{
    
}

CubeGenerator::CubeGenerator(float size, glm::vec3 color) :
    parameters{ size, color, true}
{

}

CubeGenerator::CubeGenerator(Parameters parameters) :
    parameters(parameters)
{
    
}

MeshPtr CubeGenerator::generateMesh() const
{
    float len = glm::sqrt(2 * parameters.size * parameters.size);

    const unsigned int nVertices = 24;

    std::vector<Vertex> vertices = {
        // Position                         // Color                // Normal                       // Tex coord
        {  glm::vec3( len,  len,  len),     glm::vec3(WHITE),       glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(0.f, 1.f) },  // Vertex 1     // Face 1
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(1.f, 1.f) },  // Vertex 2     // +X
        {  glm::vec3( len, -len,  len),     glm::vec3(GREEN),       glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(0.f, 0.f) },  // ...
        {  glm::vec3( len, -len, -len),     glm::vec3(YELLOW),      glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(1.f, 0.f) },

        {  glm::vec3( len, -len,  len),     glm::vec3(GREEN),       glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(1.f, 1.f) },                  // Face 2
        {  glm::vec3( len, -len, -len),     glm::vec3(YELLOW),      glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(1.f, 0.f) },                  // -Y
        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(0.f, 1.f) },
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(1.f, 0.f) },                  // Face 3
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(0.f, 0.f) },                  // -X
        {  glm::vec3(-len,  len,  len),     glm::vec3(BLUE),        glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(1.f, 1.f) },
        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len,  len),     glm::vec3(BLUE),        glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 0.f) },                  // Face 4
        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 1.f) },                  // +Y
        {  glm::vec3( len,  len,  len),     glm::vec3(WHITE),       glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 1.f) },
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(1.f, 1.f) },                  // Face 5
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(1.f, 0.f) },                  // -Z
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(0.f, 1.f) },
        {  glm::vec3( len, -len, -len),     glm::vec3(YELLOW),      glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(0.f, 0.f) },                  // Face 6
        {  glm::vec3(-len,  len,  len),     glm::vec3(BLUE),        glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(0.f, 1.f) },                  // +Z
        {  glm::vec3( len, -len,  len),     glm::vec3(GREEN),       glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(1.f, 0.f) },
        {  glm::vec3( len,  len,  len),     glm::vec3(WHITE),       glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(1.f, 1.f) }
    };

    if (parameters.useColor)
    {
        for (int i = 0; i < nVertices; i++)
        {
            vertices[i].color = parameters.color;
        }
    }

    std::vector<unsigned int> indices = {
        // Face n strip
         0,  1,  2,  3,
         4,  5,  6,  7,
         8,  9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23
    };

    std::vector<unsigned int> primitiveSizes(6, 4);
    std::vector<void*> primitiveOffsets(6);
    for (unsigned int i = 0; i < 6; i++)
    {
        primitiveOffsets[i] = (void*)(i * 4 * sizeof(unsigned int));
    }

    return std::make_shared<Mesh>(GL_TRIANGLE_STRIP, vertices, indices, primitiveSizes, primitiveOffsets);
}

}//namespace Renderboi
