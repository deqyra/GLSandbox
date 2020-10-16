#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <functional>
#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

/// @brief Collection of parameters representing a directional light.
class PointLight : public Light
{
    public:
        /// @brief Default range for point light instances.
        static constexpr float DefaultRange = 50.f;

        PointLight();

        /// @param range The distance past which the emitted light should have 
        /// noticeably faded out. Will be used to calculate factors in the light
        /// attenuation computation.
        PointLight(float range);

        /// @param ambient RGB color of the ambient component of the emitted 
        /// light.
        /// @param diffuse RGB color of the diffuse component of the emitted 
        /// light.
        /// @param specular RGB color of the specular component of the emitted 
        /// light.
        /// @param range The distance past which the emitted light should have 
        /// noticeably faded out. Will be used to calculate factors in the light
        /// attenuation computation.
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float range);

        /// @param ambient RGB color of the ambient component of the emitted 
        /// light.
        /// @param diffuse RGB color of the diffuse component of the emitted 
        /// light.
        /// @param specular RGB color of the specular component of the emitted 
        /// light.
        /// @param constant Constant term of the light attenuation computation.
        /// @param linear Linear term of the light attenuation computation.
        /// @param quadratic Quadratic term of the light attenuation computation.
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

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

        /// @brief Get a raw pointer to a new point light instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the light instance cloned from this one.
        PointLight* clone();

        /// @brief Recompute light attenuation factors according to a new range.
        ///
        /// @param range New desired range of the light.
        void setRange(float range);
};

#endif//POINT_LIGHT_HPP