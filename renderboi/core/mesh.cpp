#include "mesh.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "material.hpp"
#include "materials.hpp"
#include "vertex.hpp"

unsigned int Mesh::_count = 0;
std::unordered_map<unsigned int, unsigned int> Mesh::_arrayRefCount = std::unordered_map<unsigned int, unsigned int>();
std::unordered_map<unsigned int, unsigned int> Mesh::_bufferRefCount = std::unordered_map<unsigned int, unsigned int>();

Mesh::Mesh(unsigned int drawMode, std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
    Mesh(drawMode, vertices, indices, {(unsigned int)indices.size()}, {nullptr})
{

}

Mesh::Mesh(unsigned int drawMode, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<unsigned int> primitiveSizes, std::vector<void*> primitiveOffsets) :
    _drawMode(drawMode),
    _vertices(vertices),
    _indices(indices),
    _primitiveSizes(primitiveSizes),
    _primitiveOffsets(primitiveOffsets),
    _vao(0),
    _vbo(0),
    _ebo(0),
    id(_count++)
{
    if (primitiveSizes.size() != primitiveOffsets.size())
    {
        throw std::runtime_error("Mesh: sizes of provided arrays of primitive info do not match.");
    }

    // Setup resources on the GPU
    setupBuffers();

    // Create refcounts to resources if non existing, increase it otherwise
    if (_arrayRefCount.find(_vao) != _arrayRefCount.end())
        _arrayRefCount[_vao] = 1;
    else
        _arrayRefCount[_vao]++;

    if (_bufferRefCount.find(_vbo) != _bufferRefCount.end())
        _bufferRefCount[_vbo] = 1;
    else
        _bufferRefCount[_vbo]++;

    if (_bufferRefCount.find(_ebo) != _bufferRefCount.end())
        _bufferRefCount[_ebo] = 1;
    else
        _bufferRefCount[_ebo]++;
}

Mesh::Mesh(const Mesh& other) :
    _vertices(other._vertices),
    _indices(other._indices),
    _drawMode(other._drawMode),
    _vao(other._vao),
    _vbo(other._vbo),
    _ebo(other._ebo),
    id(_count++)
{
    // Copy everything and update refcounts
    _arrayRefCount[_vao]++;
    _bufferRefCount[_vbo]++;
    _bufferRefCount[_ebo]++;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    // Free current resources

    cleanup();
    // Copy everything
    _vertices = other._vertices;
    _indices = other._indices;
    _drawMode = other._drawMode;
    _vao = other._vao;
    _vbo = other._vbo;
    _ebo = other._ebo;

    // Update refcounts
    _arrayRefCount[_vao]++;
    _bufferRefCount[_vbo]++;
    _bufferRefCount[_ebo]++;

    return *this;
}

Mesh::~Mesh()
{
    // Free current resources
    cleanup();
}

void Mesh::cleanup()
{
    // Update all refcounts and delete resources on the GPU if appropriate
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
    // Generate arrays and buffers on the GPU
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
  
    // Bind VAO
    glBindVertexArray(_vao);

    // Bind VBO and send vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);  

    // Bind EBO and send index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // Setup vertex attributes:

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
    glMultiDrawElements(
        _drawMode, 
        (GLsizei*) &(_primitiveSizes[0]), 
        GL_UNSIGNED_INT, 
        (void* const*) &(_primitiveOffsets[0]), 
        (GLsizei) _primitiveSizes.size()
    );
}
