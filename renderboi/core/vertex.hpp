#ifndef CORE__VERTEX_HPP
#define CORE__VERTEX_HPP

#include <glm/glm.hpp>

/// @brief A vertex with basic attributes.
struct Vertex
{
    /// @brief The 3D position of the vertex.
    glm::vec3 position;

    /// @brief The RGB color of the vertex.
    glm::vec3 color;

    /// @brief The normal vector of the vertex.
    glm::vec3 normal;

    /// @brief Texture coordinates associated with the vertex.
    glm::vec2 texCoord;
};

#endif//CORE__VERTEX_HPP