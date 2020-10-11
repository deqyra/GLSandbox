#ifndef CORE__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP
#define CORE__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP

#include "mesh_type.hpp"
#include "axes_generator.hpp"
#include "cube_generator.hpp"
#include "tetrahedron_generator.hpp"
#include "torus_generator.hpp"

template<MeshType T>
struct TypeToGenMapping
{
    typedef nullptr_t GenType;
};

template<>
struct TypeToGenMapping<MeshType::Axes>
{
    typedef AxesGenerator GenType;
};

template<>
struct TypeToGenMapping<MeshType::Cube>
{
    typedef CubeGenerator GenType;
};

template<>
struct TypeToGenMapping<MeshType::Tetrahedron>
{
    typedef TetrahedronGenerator GenType;
};

template<>
struct TypeToGenMapping<MeshType::Torus>
{
    typedef TorusGenerator GenType;
};

#endif//CORE__MESH_GENERATORS__TYPE_TO_GEN_MAPPING_HPP