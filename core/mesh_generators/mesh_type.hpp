#ifndef CORE__MESH_GENERATORS__MESH_TYPE_HPP
#define CORE__MESH_GENERATORS__MESH_TYPE_HPP

/// @brief Collection of litterals describing the different shapes a mesh can 
// come in. Each of these can be mapped to a concrete MeshGenerator subclass.
enum class MeshType
{
    Axes,
    Cube,
    Tetrahedron,
    Torus
};

#endif//CORE__MESH_GENERATORS__MESH_TYPE_HPP