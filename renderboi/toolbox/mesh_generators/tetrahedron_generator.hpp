#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP

#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>

#include "../interfaces/mesh_generator.hpp"

namespace Renderboi
{

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
    MeshPtr generateMesh() const override;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
