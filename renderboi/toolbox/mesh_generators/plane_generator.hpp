#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__PLANE_GENERATOR_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__PLANE_GENERATOR_HPP

#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>

#include "../mesh_generator.hpp"

/// @brief Generates vertices for a tiled plane.
class PlaneGenerator : public MeshGenerator
{
public:
    /// @brief Struct packing together the parameters of the vertex
    /// generation.
    struct Parameters
    {
        /// @brief Size the tiles of the plane will have along the X 
        /// direction.
        float tileSizeX;

        /// @brief Size the tiles of the plane will have along the Y 
        /// direction.
        float tileSizeY;

        /// @brief Amount of tiles in the plane along the X direction.
        unsigned int tileAmountX;

        /// @brief Amount of tiles in the plane along the Y direction.
        unsigned int tileAmountY;

        /// @brief Size of the texture along the X direction. Affects the
        /// generated texture coordinates. The bigger the provided texture
        /// size, the smaller the texture coordinates will be. Specifying 0 
        /// or a negative value will cause the texture coordinates to be 
        /// spread evenly from 0 to 1 on the plane.
        float xTexSize;

        /// @brief Size of the texture along the Y direction. Affects the
        /// generated texture coordinates. The bigger the provided texture
        /// size, the smaller the texture coordinates will be. Specifying 0 
        /// or a negative value will cause the texture coordinates to be 
        /// spread evenly from 0 to 1 on the plane.
        float yTexSize;

        /// @brief Offset by which to shift X texture coordinates.
        float xTexCoordOffset;

        /// @brief Offset by which to shift Y texture coordinates.
        float yTexCoordOffset;

        /// @brief Whether or not to invert the tex coords along the X axis.
        bool invertXTexCoords;

        /// @brief Whether or not to invert the tex coords along the Y axis.
        bool invertYTexCoords;
        
        /// @brief Angle (in radians) by which to rotate the texture 
        /// coordinates in the trigonometric direction. The rotation occurs
        /// after the reversal of coordinates if specified.
        float texRotation;

        /// @brief RGB color of the generated vertices.
        glm::vec3 color;
    };

    PlaneGenerator();

    /// @param parameters Parameters of the vertex generation.
    PlaneGenerator(Parameters parameters);
    
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
    MeshPtr generatePtr() const;
};

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__PLANE_GENERATOR_HPP
