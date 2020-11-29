#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP

#include <renderboi/core/mesh.hpp>

#include "../mesh_generator.hpp"

/// @brief Generates vertices for the three regular X, Y and Z axes,
/// respectively colored red, green and blue.
class AxesGenerator : public MeshGenerator
{
    public:
        /// @brief Default length the axes will have.
        static constexpr float DefaultAxisLength = 1.f;

        /// @brief Struct packing together the parameters of the vertex
        /// generation.
        struct Parameters
        {
            /// @brief Length the axes will have.
            float axisLength;
        };

        AxesGenerator();

        /// @param length Length the axes will have.
        AxesGenerator(float length);

        /// @param parameters Parameters of the vertex generation.
        AxesGenerator(Parameters parameters);

        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generatePtr();
        
        /// @brief Parameters of the vertex generation.
        Parameters parameters;
};

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP
