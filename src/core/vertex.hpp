#ifndef CORE__VERTEX_HPP
#define CORE__VERTEX_HPP

#include <glm/glm.hpp>

// A vertex with basic attributes
struct Vertex
{
    // The 3D position of the vertex
    glm::vec3 position;
    // The RGB color of the vertex
    glm::vec3 color;
    // The normal vector of the vertex
    glm::vec3 normal;
    // Texture coordinates associated with the vertex
    glm::vec2 texCoord;
};

#endif//CORE__VERTEX_HPP