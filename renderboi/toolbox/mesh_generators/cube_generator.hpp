#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__CUBE_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__CUBE_GENERATOR_HPP

#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>

#include "../mesh_generator.hpp"

/// @brief Generates vertices for a multi- or single-colored cube.
class CubeGenerator : public MeshGenerator
{
    public:
        /// @brief Default size the cube will have (radius of encompassing
        /// sphere).
        static constexpr float DefaultSize = 1.f;

        /// @brief Struct packing together the parameters of the vertex
        /// generation.
        struct Parameters
        {
            /// @brief Size the cube will have (radius of encompassing sphere).
            float size;

            /// @brief RGB color of the generated vertices.
            glm::vec3 color;
            
            /// @brief Whether or not to use the provided color parameter. If 
            /// false, the cube will be multicolored.
            bool useColor;
        };

        CubeGenerator();

        /// @param size Size the cube will have (radius of encompassing sphere).
        CubeGenerator(float size);

        /// @param size Size the cube will have (radius of encompassing sphere).
        /// @param color RGB color of the generated vertices.
        CubeGenerator(float size, glm::vec3 color);

        /// @param parameters Parameters of the vertex generation.
        CubeGenerator(Parameters parameters);
        
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

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__CUBE_GENERATOR_HPP
