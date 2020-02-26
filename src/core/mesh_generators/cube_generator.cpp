/**
    GLTest, cube.cpp
    Purpose: Implementation of class CubeGenerator. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include "cube_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../vertex.hpp"

#include "../../tools/gl_utils.hpp"

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

CubeGenerator::CubeGenerator(float size, float r, float g, float b) :
    parameters{ size, glm::vec3(r, g, b), true }
{
    
}

CubeGenerator::CubeGenerator(Parameters parameters) :
    parameters(parameters)
{
    
}

Mesh CubeGenerator::generate()
{
    return *generatePtr();
}

MeshPtr CubeGenerator::generatePtr()
{
    float len = parameters.size / 2.f;

    const unsigned int nVertices = 24;

    std::vector<Vertex> vertices = {
        // Position                         // Color                // Normal                       // Tex coord
        {  glm::vec3( len,  len,  len),     glm::vec3(WHITE),       glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(0.f, 1.f) },  // Vertex 1     // Face 1
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(1.f, 1.f) },  // Vertex 2     // +X
        {  glm::vec3( len, -len,  len),     glm::vec3(GREEN),       glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(0.f, 0.f) },  // ...
        {  glm::vec3( len, -len, -len),     glm::vec3(BLUE),        glm::vec3( 1.f,  0.f,  0.f),    glm::vec2(1.f, 0.f) },

        {  glm::vec3( len, -len,  len),     glm::vec3(GREEN),       glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(1.f, 1.f) },                  // Face 2
        {  glm::vec3( len, -len, -len),     glm::vec3(BLUE),        glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(1.f, 0.f) },                  // -Y
        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(0.f, 1.f) },
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3( 0.f, -1.f,  0.f),    glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(1.f, 0.f) },                  // Face 3
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(0.f, 0.f) },                  // -X
        {  glm::vec3(-len,  len,  len),     glm::vec3(YELLOW),      glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(1.f, 1.f) },
        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3(-1.f,  0.f,  0.f),    glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len,  len),     glm::vec3(YELLOW),      glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 0.f) },                  // Face 4
        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 1.f) },                  // +Y
        {  glm::vec3( len,  len,  len),     glm::vec3(WHITE),       glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(1.f, 1.f) },
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 0.f,  1.f,  0.f),    glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len, -len),     glm::vec3(MAGENTA),     glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(1.f, 1.f) },                  // Face 5
        {  glm::vec3(-len, -len, -len),     glm::vec3(BLACK),       glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(1.f, 0.f) },                  // -Z
        {  glm::vec3( len,  len, -len),     glm::vec3(RED),         glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(0.f, 1.f) },
        {  glm::vec3( len, -len, -len),     glm::vec3(BLUE),        glm::vec3( 0.f,  0.f, -1.f),    glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     glm::vec3(CYAN),        glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(0.f, 0.f) },                  // Face 6
        {  glm::vec3(-len,  len,  len),     glm::vec3(YELLOW),      glm::vec3( 0.f,  0.f,  1.f),    glm::vec2(0.f, 1.f) },                  // +Z
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
        // Face n strip     // Insert degenerate triangles to move onto the next face strip
         0,  1,  2,  3,      3,  4,     // Face 1
         4,  5,  6,  7,      7,  8,     // Face 2
         8,  9, 10, 11,     11, 12,     // ...
        12, 13, 14, 15,     15, 16,
        16, 17, 18, 19,     19, 20,
        20, 21, 22, 23
    };

    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLE_STRIP);
}