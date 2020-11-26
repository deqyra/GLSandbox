#ifndef CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP
#define CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

#include <glm/glm.hpp>

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

        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generatePtr();
        
        /// @brief Parameters of the vertex generation.
        Parameters parameters;
};

#endif//CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP
