#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP

#include <renderboi/core/mesh.hpp>

#include "../interfaces/mesh_generator.hpp"

namespace Renderboi
{

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

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__AXES_GENERATOR_HPP
