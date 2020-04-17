#ifndef CORE__MESH_GENERATOR_HPP
#define CORE__MESH_GENERATOR_HPP

#include "mesh.hpp"

// Any class which can generate a mesh and return a pointer to it
class MeshGenerator
{
    public:
        virtual MeshPtr generatePtr() = 0;
};

#endif//CORE__MESH_GENERATOR_HPP