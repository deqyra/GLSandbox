#ifndef CORE__MESH_GENERATORS__TORUS_GENERATOR_HPP
#define CORE__MESH_GENERATORS__TORUS_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

class TorusGenerator : public MeshGenerator
{
    public:
        static constexpr float DefaultToroidalRadius = 2.f;
        static constexpr float DefaultPoloidalRadius = 0.5f;
        static constexpr unsigned int DefaultToroidalVertexResolution = 36;
        static constexpr unsigned int DefaultPoloidalVertexResolution = 12;

        struct Parameters
        {
            float toroidalRadius;
            float poloidalRadius;
            unsigned int toroidalVertexRes;
            unsigned int poloidalVertexRes;
        };

        TorusGenerator();
        TorusGenerator(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes = DefaultToroidalVertexResolution, unsigned int poloidalVertexRes = DefaultPoloidalVertexResolution);
        TorusGenerator(Parameters parameters);

        virtual MeshPtr generatePtr();

        Parameters parameters;
};

#endif//CORE__MESH_GENERATORS__TORUS_GENERATOR_HPP
