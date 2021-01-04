#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP

namespace Renderboi
{

/// @brief Collection of literals describing the different shapes a mesh can 
// come in. Each of these can be mapped to a concrete MeshGenerator subclass.
enum class MeshType
{
    Axes,
    Cube,
    Plane,
    Tetrahedron,
    Torus
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP