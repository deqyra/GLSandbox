#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <glm/glm.hpp>

#include "light.hpp"
#include "light_type.hpp"

/// @brief Collection of parameters representing a directional light.
class DirectionalLight : public Light
{
    public:
        /// @param direction Vector representing the direction of the light.
        DirectionalLight(glm::vec3 direction);

        /// @param direction Vector representing the direction of the light.
        /// @param ambient RGB color of the ambient component of the emitted 
        /// light.
        /// @param diffuse RGB color of the diffuse component of the emitted 
        /// light.
        /// @param specular RGB color of the specular component of the emitted 
        /// light.
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        /// @brief Direction of the light.
        glm::vec3 direction;

        /// @brief RGB color of the ambient component of the emitted light.
        glm::vec3 ambient;

        /// @brief RGB color of the diffuse component of the emitted light.
        glm::vec3 diffuse;

        /// @brief RGB color of the specular component of the emitted light.
        glm::vec3 specular;

        /////////////////////////////////////
        ///                               ///
        /// Methods overridden from Light ///
        ///                               ///
        /////////////////////////////////////

        /// @brief Get a raw pointer to a new directional light instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the light instance cloned from this one.
        DirectionalLight* clone();
};

#endif//DIRECTIONAL_LIGHT_HPP