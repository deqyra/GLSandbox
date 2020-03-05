/**
    GLTest, axes.hpp
    Purpose: Define a mesh class to represent 3D axes.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#ifndef AXES_GENERATOR_HPP
#define AXES_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

class AxesGenerator : public MeshGenerator
{
    public:
        static constexpr float DefaultAxisLength = 1.f;

        struct Parameters
        {
            float axisLength;
        };

        AxesGenerator();
        AxesGenerator(float size);
        AxesGenerator(Parameters parameters);

        virtual MeshPtr generatePtr();
        
        Parameters parameters;
};

#endif//AXES_GENERATOR_HPP
