#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP

#include "mesh_type.hpp"
#include "axes_generator.hpp"
#include "cube_generator.hpp"
#include "plane_generator.hpp"
#include "tetrahedron_generator.hpp"
#include "torus_generator.hpp"

/// @brief Used for mapping, at compile-time, a MeshType to the corresponding 
/// MeshGenerator concrete subclass. Use inner typedef GenType to get the 
/// actual type.
///
/// @tparam T Literal describing the mesh type whose mesh generator to obtain.
template<MeshType T>
struct TypeToGenMapping
{
    /// @brief Type of the concrete MeshGenerator subclass mapped to MeshType T.
    using GenType = nullptr_t;
};

template<>
struct TypeToGenMapping<MeshType::Axes>
{
    using GenType = AxesGenerator;
};

template<>
struct TypeToGenMapping<MeshType::Cube>
{
    using GenType = CubeGenerator;
};

template<>
struct TypeToGenMapping<MeshType::Plane>
{
    using GenType = PlaneGenerator;
};

template<>
struct TypeToGenMapping<MeshType::Tetrahedron>
{
    using GenType = TetrahedronGenerator;
};

template<>
struct TypeToGenMapping<MeshType::Torus>
{
    using GenType = TorusGenerator;
};

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP