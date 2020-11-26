#ifndef CORE__MESH_GENERATOR_HPP
#define CORE__MESH_GENERATOR_HPP

#include "mesh.hpp"

/// @brief Interface for classes which can generate parameterized vertex data.
class MeshGenerator
{
    public:
        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generatePtr() = 0;
};

#endif//CORE__MESH_GENERATOR_HPP