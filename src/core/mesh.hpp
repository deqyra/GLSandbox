/**
    GLTest, mesh.hpp
    Purpose: Define an abstract class to represent any mesh.

    @author François Brachais (deqyra)
    @version 1.0 07/02/2020
 */
#ifndef MESH_HPP
#define MESH_HPP

#include "positioned_object.hpp"
#include "shader.hpp"
#include "material.hpp"

#include <string>

class Mesh : public PositionedObject
{
    private:
        static unsigned int _count;

    protected:
        bool _buffersSetUp;

    public:
        Mesh();

        virtual void setupBuffers() = 0;
        virtual void draw() = 0;

        const unsigned int id;
        Material material;
};

#endif//MESH_HPP
