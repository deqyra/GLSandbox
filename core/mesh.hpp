#ifndef CORE__MESH_HPP
#define CORE__MESH_HPP

#include <string>
#include <vector>
#include <memory>

#include <glad/gl.h>

#include "material.hpp"
#include "vertex.hpp"

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;
using MeshWPtr = std::weak_ptr<Mesh>;

// Any mesh composed of vertices to be rendered by indexed drawing
class Mesh
{
    private:
        // Keeps track of how many Mesh objects were created (used as an ID system)
        static unsigned int _count;

        // Map storing how many mesh instances are referencing a VAO resource on the GPU:
        // VAO ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _arrayRefCount;

        // Map storing how many mesh instances are referencing a buffer resource on the GPU:
        // Buffer ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _bufferRefCount;

        // Update reference counts before destruction
        void cleanup();
        // Send mesh data to the GPU
        void setupBuffers();

    protected:
        // Vertex data of the mesh
        std::vector<Vertex> _vertices;
        // Index data of the mesh
        std::vector<unsigned int> _indices;
        // Draw mode
        unsigned int _drawMode;

        // Handle to a VAO on the GPU
        unsigned int _vao;
        // Handle to a VBO on the GPU
        unsigned int _vbo;
        // Handle to an EBO on the GPU
        unsigned int _ebo;

    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode);
        Mesh(const Mesh& other);
        Mesh& operator=(const Mesh& other);
        ~Mesh();

        // Issue GPU draw commands
        void draw();

        // ID of the Mesh object
        const unsigned int id;
};

#endif//CORE__MESH_HPP
