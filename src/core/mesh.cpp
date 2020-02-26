/**
    GLTest, mesh.hpp
    Purpose: Implement part of class Mesh. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 13/02/2020
 */

#include "mesh.hpp"
#include "materials.hpp"

unsigned int Mesh::_count = 0;
std::unordered_map<unsigned int, unsigned int> Mesh::_arrayRefCount = std::unordered_map<unsigned int, unsigned int>();
std::unordered_map<unsigned int, unsigned int> Mesh::_bufferRefCount = std::unordered_map<unsigned int, unsigned int>();

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int drawMode, Material material) :
    _vertices(vertices),
    _indices(indices),
    _drawMode(drawMode),
    _vao(0),
    _vbo(0),
    _ebo(0),
    id(_count++),
    material(material)
{
    setupBuffers();

    _arrayRefCount[_vao]++;
    _bufferRefCount[_vbo]++;
    _bufferRefCount[_ebo]++;
}

Mesh::Mesh(const Mesh& other) :
    _vertices(other._vertices),
    _indices(other._indices),
    _drawMode(other._drawMode),
    _vao(other._vao),
    _vbo(other._vbo),
    _ebo(other._ebo),
    id(_count++),
    material(other.material)
{
    _arrayRefCount[_vao]++;
    _bufferRefCount[_vbo]++;
    _bufferRefCount[_ebo]++;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    cleanup();

    _vertices = other._vertices;
    _indices = other._indices;
    _drawMode = other._drawMode;
    _vao = other._vao;
    _vbo = other._vbo;
    _ebo = other._ebo;
    material = other.material;

    _arrayRefCount[_vao]++;
    _bufferRefCount[_vbo]++;
    _bufferRefCount[_ebo]++;

    return *this;
}

Mesh::~Mesh()
{
    cleanup();
}

void Mesh::cleanup()
{
    unsigned int count = --_arrayRefCount[_vao];
    if (!count)
    {
        glDeleteVertexArrays(1, &_vao);
    }
    
    count = --_bufferRefCount[_vbo];
    if (!count)
    {
        glDeleteBuffers(1, &_vbo);
    }
    
    count = --_bufferRefCount[_ebo];
    if (!count)
    {
        glDeleteBuffers(1, &_ebo);
    }
}

void Mesh::setupBuffers()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
  
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // Vertex colors
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // Vertex normals
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Vertex texture coords
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
}

void Mesh::draw()
{
    // Draw mesh
    glBindVertexArray(_vao);
    glDrawElements(_drawMode, (unsigned int) _indices.size(), GL_UNSIGNED_INT, 0);
}

MeshPtr Mesh::getPtr(Mesh mesh)
{
    return std::make_shared<Mesh>(mesh);
}