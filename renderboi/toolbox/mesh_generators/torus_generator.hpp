#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__TORUS_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__TORUS_GENERATOR_HPP

#include <renderboi/core/mesh.hpp>

#include "../mesh_generator.hpp"

/// @brief Generates the vertex data for a torus.
class TorusGenerator : public MeshGenerator
{
    public:
        /// @brief Default toroidal ("large") radius of the torus.
        static constexpr float DefaultToroidalRadius = 2.f;

        /// @brief Default poloidal ("small") radius of the torus.
        static constexpr float DefaultPoloidalRadius = 0.5f;

        /// @brief How many vertices to use by default along the toroidal 
        /// ("large") circumference of the torus.
        static constexpr unsigned int DefaultToroidalVertexResolution = 36;

        /// @brief How many vertices to use by default along the poloidal 
        /// ("small") circumference of the torus.
        static constexpr unsigned int DefaultPoloidalVertexResolution = 12;

        /// @brief Struct packing together the parameters of the vertex
        /// generation.
        struct Parameters
        {
            /// @brief Toroidal ("large") radius of the torus.
            float toroidalRadius;

            /// @brief Poloidal ("small") radius of the torus.
            float poloidalRadius;

            /// @brief How many vertices to use along the toroidal ("large") 
            /// circumference of the torus.
            unsigned int toroidalVertexRes;

            /// @brief How many vertices to use along the poloidal ("small") 
            /// circumference of the torus.
            unsigned int poloidalVertexRes;
        };

        TorusGenerator();

        /// @param toroidalRadius Default toroidal ("large") radius of the 
        /// torus.
        /// @param poloidalRadius Default poloidal ("small") radius of the 
        /// torus.
        /// @param toroidalVertexRes How many vertices to use by default along 
        /// the toroidal ("large") circumference of the torus.
        /// @param poloidalVertexRes How many vertices to use by default along 
        /// the poloidal ("small") circumference of the torus.
        TorusGenerator(float toroidalRadius, float poloidalRadius, unsigned int toroidalVertexRes = DefaultToroidalVertexResolution, unsigned int poloidalVertexRes = DefaultPoloidalVertexResolution);

        /// @param parameters Parameters of the vertex generation.
        TorusGenerator(Parameters parameters);

        /// @brief Parameters of the vertex generation.
        Parameters parameters;

        /////////////////////////////////////////////
        ///                                       ///
        /// Methods overridden from MeshGenerator ///
        ///                                       ///
        /////////////////////////////////////////////

        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        MeshPtr generatePtr();
};

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__TORUS_GENERATOR_HPP
