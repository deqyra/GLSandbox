#ifndef CORE__MESH_GENERATORS__AXES_GENERATOR_HPP
#define CORE__MESH_GENERATORS__AXES_GENERATOR_HPP

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

#endif//CORE__MESH_GENERATORS__AXES_GENERATOR_HPP
