#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

namespace Renderboi
{

/// @brief Collection of parameters representing a directional light.
class PointLight : public Light
{
public:
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    PointLight(const float range);

    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    PointLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const float range);

    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param constant Constant term of the light attenuation computation.
    /// @param linear Linear term of the light attenuation computation.
    /// @param quadratic Quadratic term of the light attenuation computation.
    PointLight(
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float constant,
        const float linear,
        const float quadratic
    );

    /// @brief RGB color of the ambient component of the emitted light.
    glm::vec3 ambient;
    
    /// @brief RGB color of the diffuse component of the emitted light.
    glm::vec3 diffuse;
    
    /// @brief RGB color of the specular component of the emitted light.
    glm::vec3 specular;

    /// @brief Constant term of the light attenuation computation.
    float constant;
    
    /// @brief Linear term of the light attenuation computation.
    float linear;
    
    /// @brief Quadratic term of the light attenuation computation.
    float quadratic;

    /// @brief Recompute light attenuation factors according to a new range.
    ///
    /// @param range New desired range of the light.
    void setRange(const float range);

    /////////////////////////////////////
    ///                               ///
    /// Methods overridden from Light ///
    ///                               ///
    /////////////////////////////////////

    /// @brief Get a raw pointer to a new point light instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @return A raw pointer to the light instance cloned from this one.
    PointLight* clone() const override;
};

}//namespace Renderboi

#endif//POINT_LIGHT_HPP