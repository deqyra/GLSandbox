#ifndef RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP

#include <renderboi/core/mesh.hpp>

/// @brief Interface for classes which can generate parameterized vertex data.
class MeshGenerator
{
    public:
        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generatePtr() const = 0;
};

#endif//RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP