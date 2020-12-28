#ifndef RENDERBOI__CORE__LIGHTS__SPOT_LIGHT_HPP
#define RENDERBOI__CORE__LIGHTS__SPOT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

/// @brief Collection of parameters representing a spot light.
class SpotLight : public Light
{
public:
    /// @brief Default range for spot light instances.
    static constexpr float DefaultRange = 50.f;

    /// @brief Default angle at which the light starts fading out.
    static constexpr float DefaultInnerCutoff = glm::radians(15.f);
    
    /// @brief Default angle at which the light has completely faded out.
    static constexpr float DefaultOuterCutoff = glm::radians(20.f);

    /// @param direction Vector representing the direction of the light.
    SpotLight(const glm::vec3 direction);

    /// @param direction Vector representing the direction of the light.
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    SpotLight(const glm::vec3 direction, const float range);

    /// @param direction Vector representing the direction of the light.
    /// @param innerCutoff Angle at which the light should start fading out.
    /// @param outerCutoff Angle at which the light should have completely 
    /// faded out.
    SpotLight(const glm::vec3 direction, const float innerCutoff, const float outerCutoff);

    /// @param direction Vector representing the direction of the light.
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    /// @param innerCutoff Angle at which the light should start fading out.
    /// @param outerCutoff Angle at which the light should have completely 
    /// faded out.
    SpotLight(const glm::vec3 direction, const float range, const float innerCutoff, const float outerCutoff);

    /// @param direction Vector representing the direction of the light.
    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    SpotLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular);

    /// @param direction Vector representing the direction of the light.
    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    SpotLight(
        const glm::vec3 direction,
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float range
    );

    /// @param direction Vector representing the direction of the light.
    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param innerCutoff Angle at which the light should start fading out.
    /// @param outerCutoff Angle at which the light should have completely 
    /// faded out.
    SpotLight(
        const glm::vec3 direction,
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float innerCutoff,
        const float outerCutoff
    );

    /// @param direction Vector representing the direction of the light.
    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param innerCutoff Angle at which the light should start fading out.
    /// @param outerCutoff Angle at which the light should have completely 
    /// faded out.
    /// @param range The distance past which the emitted light should have 
    /// noticeably faded out. Will be used to calculate factors in the light
    /// attenuation computation.
    SpotLight(
        const glm::vec3 direction,
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float innerCutoff,
        const float outerCutoff,
        const float range
    );

    /// @param direction Vector representing the direction of the light.
    /// @param ambient RGB color of the ambient component of the emitted 
    /// light.
    /// @param diffuse RGB color of the diffuse component of the emitted 
    /// light.
    /// @param specular RGB color of the specular component of the emitted 
    /// light.
    /// @param innerCutoff Angle at which the light should start fading out.
    /// @param outerCutoff Angle at which the light should have completely 
    /// faded out.
    /// @param constant Constant term of the light attenuation computation.
    /// @param linear Linear term of the light attenuation computation.
    /// @param quadratic Quadratic term of the light attenuation computation.
    SpotLight(
        const glm::vec3 direction,
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float innerCutoff,
        const float outerCutoff,
        const float constant,
        const float linear,
        const float quadratic
    );

    /// @brief Direction the spot light is facing.
    glm::vec3 direction;
    
    /// @brief RGB color of the ambient component of the emitted light.
    glm::vec3 ambient;
    
    /// @brief RGB color of the diffuse component of the emitted light.
    glm::vec3 diffuse;
    
    /// @brief RGB color of the specular component of the emitted light.
    glm::vec3 specular;
    
    /// @brief Angle at which the light starts fading out.
    float innerCutoff;
    
    /// @brief Angle at which the light has completely faded out.
    float outerCutoff;

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

    /// @brief Get a raw pointer to a new spot light instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @return A raw pointer to the light instance cloned from this one.
    SpotLight* clone() const override;
};

#endif//RENDERBOI__CORE__LIGHTS__SPOT_LIGHT_HPP