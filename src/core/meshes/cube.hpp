/**
    GLTest, cube.hpp
    Purpose: Define a mesh class to represent a cube.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#ifndef CUBE_HPP
#define CUBE_HPP

#include "../mesh.hpp"

class Cube : public Mesh
{
    private:
        float _vertices[48];
        unsigned int _indices[18];

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    public:
        Cube();
        Cube(float size);
        Cube(float size, float r, float g, float b);
        ~Cube();

        void setupBuffers();
        void draw();
};

#endif//CUBE_HPP
