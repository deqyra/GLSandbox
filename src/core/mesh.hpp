/**
    GLTest, mesh.hpp
    Purpose: Define an abstract class to represent any mesh.

    @author François Brachais (deqyra)
    @version 1.0 07/02/2020
 */
#ifndef MESH_HPP
#define MESH_HPP

#include "../../include/glad/glad.h"

#include "positioned_object.hpp"
#include "vertex.hpp"
#include "material.hpp"

#include <string>
#include <vector>

class Mesh : public PositionedObject
{
    private:
        static unsigned int _count;

    protected:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
        unsigned int _drawMode;

        bool _buffersSetUp;
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode, Material material, bool skipDataCheck);

    public:
        const unsigned int id;
        Material material;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode, Material material = Material());

        void setupBuffers();
        void draw();
};

#endif//MESH_HPP
