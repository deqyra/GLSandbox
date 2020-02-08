/**
    GLTest, mesh.hpp
    Purpose: Define an abstract class to represent any mesh.

    @author François Brachais (deqyra)
    @version 1.0 07/02/2020
 */
#ifndef MESH_HPP
#define MESH_HPP

#include "positioned_object.hpp"

class Mesh : public PositionedObject
{
    public:
        virtual void setupBuffers() = 0;
        virtual void draw() = 0;
};

#endif//MESH_HPP
