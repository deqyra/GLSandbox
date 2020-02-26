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
#include <memory>

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh : public PositionedObject
{
    private:
        static unsigned int _count;

        // Structure storing how many mesh instances are referencing a VAO resource on the GPU:
        // VAO ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _arrayRefCount;

        // Structure storing how many mesh instances are referencing a buffer resource on the GPU:
        // Buffer ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _bufferRefCount;

        void cleanup();

    protected:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
        unsigned int _drawMode;

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode, Material material = Material());
        Mesh(const Mesh& other);
        Mesh& operator=(const Mesh& other);
        ~Mesh();

        void setupBuffers();
        void draw();

        static MeshPtr getPtr(Mesh mesh);

        const unsigned int id;
        Material material;
};

#endif//MESH_HPP
