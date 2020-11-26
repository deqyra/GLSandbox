#ifndef LIGHT_TYPE_HPP
#define LIGHT_TYPE_HPP

/// @brief Collection of literals describing the available types of light.
enum class LightType
{
    /// @brief Emits light in all directions from its position.
    PointLight,
    /// @brief Emits light in the same direction for the whole scene.
    DirectionalLight,
    /// @brief Emits light in a cone from its position.
    SpotLight
};

#endif//LIGHT_TYPE_HPP
