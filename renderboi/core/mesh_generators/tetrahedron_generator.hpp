#ifndef CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
#define CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

#include <glm/glm.hpp>

/// @brief Generates vertices for a multi- or single-colored tetrahedron.
class TetrahedronGenerator : public MeshGenerator
{
    public:
        /// @brief Default size the tetrahedron will have (radius of 
        /// encompassing sphere).
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
            /// false, the tetrahedron will be multicolored.
            bool useColor;
        };

        TetrahedronGenerator();

        /// @param size Size the tetrahedron will have (radius of encompassing 
        /// sphere).
        TetrahedronGenerator(float size);

        /// @param size Size the tetrahedron will have (radius of encompassing 
        /// sphere).
        /// @param color RGB color of the generated vertices.
        TetrahedronGenerator(float size, glm::vec3 color);

        /// @param parameters Parameters of the vertex generation.
        TetrahedronGenerator(Parameters parameters);

        /// @brief Generate the vertex data, put it in a new mesh object and 
        /// return it.
        ///
        /// @return A pointer to the mesh containing the generated vertices.
        virtual MeshPtr generatePtr();
        
        /// @brief Parameters of the vertex generation.
        Parameters parameters;
};

#endif//CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
