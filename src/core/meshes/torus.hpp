/**
    GLTest, torus.hpp
    Purpose: Define a mesh class to represent a torus.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#ifndef TORUS_HPP
#define TORUS_HPP

#include "../mesh.hpp"

#define DEFAULT_TOROIDAL_RADIUS 2.f
#define DEFAULT_POLOIDAL_RADIUS 0.5f
#define DEFAULT_TOROIDAL_VERTEX_RESOLUTION 36
#define DEFAULT_POLOIDAL_VERTEX_RESOLUTION 12

class Torus : public Mesh
{
    private:
        unsigned int _toroidalVertexRes;
        unsigned int _poloidalVertexRes;

        float* _vertices;
        unsigned int* _indices;

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

        void fillVertexData(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes, unsigned int poloidalVertexRes);

    public:
        Torus();
        Torus(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes = DEFAULT_TOROIDAL_VERTEX_RESOLUTION, unsigned int poloidalVertexRes = DEFAULT_POLOIDAL_VERTEX_RESOLUTION);
        ~Torus();

        void setupBuffers();
        void draw();
};

#endif//TORUS_HPP
