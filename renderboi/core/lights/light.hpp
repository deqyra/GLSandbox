#ifndef CORE__LIGHT_HPP
#define CORE__LIGHT_HPP

#include <memory>

#include "light_type.hpp"

/// @brief Abstract light.
class Light
{
    private:
        Light() = delete;

        /// @brief Keeps track of how many instances were created (used as a
        /// unique ID system).
        static unsigned int _count;

    protected:
        /// @param type Literal describing which type the light will be.
        Light(LightType type);

    public:
        /// @brief ID of the light instance.
        const unsigned int id;

        /// @brief Type of the light instance.
        const LightType lightType;

        /// @brief Get a raw pointer to a new light instance cloned from this
        /// one. Ownership and responsibility for the allocated resources are 
        /// fully transferred to the caller.
        ///
        /// @return A raw pointer to the light instance cloned from this one.
        virtual Light* clone() = 0;
};

using LightPtr = std::shared_ptr<Light>;
using LightWPtr = std::weak_ptr<Light>;

#endif//CORE__LIGHT_HPP
