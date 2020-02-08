/**
    GLTest, axes.hpp
    Purpose: Define a mesh class to represent 3D axes.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#ifndef AXES_HPP
#define AXES_HPP

#include "../mesh.hpp"

class Axes : public Mesh
{
    private:
        float _vertices[36];
        unsigned int _indices[6];

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    public:
        Axes(float size);
        ~Axes();

        void setupBuffers();
        void draw();
};

#endif//AXES_HPP
