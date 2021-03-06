#ifndef RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP

#include <renderboi/core/mesh.hpp>

namespace Renderboi
{

/// @brief Interface for classes which can generate parameterized vertex data.
class MeshGenerator
{
    public:
        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generateMesh() const = 0;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__MESH_GENERATOR_HPP