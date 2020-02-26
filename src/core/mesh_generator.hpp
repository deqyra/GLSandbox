#ifndef MESH_GENERATOR_HPP
#define MESH_GENERATOR_HPP

#include "mesh.hpp"

class MeshGenerator
{
    public:
        virtual Mesh generate() = 0;
        virtual MeshPtr generatePtr() = 0;
};

#endif//MESH_GENERATOR_HPP