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

/// @brief A mesh holding vertices to be rendered using indexed drawing.
class Mesh
{
    private:
        /// @brief Keeps track of how many instances were created (used as a 
        /// unique ID system).
        static unsigned int _count;

        /// @brief Map storing how many mesh instances are referencing a VAO 
        /// resource on the GPU (VAO ID => reference count).
        static std::unordered_map<unsigned int, unsigned int> _arrayRefCount;

        /// @brief Map storing how many mesh instances are referencing a VBO 
        /// resource on the GPU (VBO ID => reference count).
        static std::unordered_map<unsigned int, unsigned int> _bufferRefCount;

        /// @brief Free resources before instance destruction.
        void cleanup();

        /// @brief Send vertex data to the GPU.
        void setupBuffers();

    protected:
        /// @brief Vertices the mesh is made of.
        std::vector<Vertex> _vertices;
        /// @brief Vertex indices teling how to draw the mesh.
        std::vector<unsigned int> _indices;
        /// @brief Draw policy to use when drawing.
        unsigned int _drawMode;

        /// @brief Handle to the VAO on the GPU.
        unsigned int _vao;
        /// @brief Handle to the VBO on the GPU.
        unsigned int _vbo;
        /// @brief Handle to the EBO on the GPU.
        unsigned int _ebo;

    public:
        Mesh(const Mesh& other);

        /// @param vertices Vertex data of the mesh.
        /// @param indices Vertex indices telling how to draw the mesh.
        /// @param drawMode Draw policy to use when drawing.
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode);

        ~Mesh();

        Mesh& operator=(const Mesh& other);

        /// @brief Issue GPU draw commands.
        void draw();

        /// @brief ID of the Mesh instance.
        const unsigned int id;
};

#endif//CORE__MESH_HPP
