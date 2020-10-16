#ifndef CORE__SCENE__COMPONENT_TYPE_HPP
#define CORE__SCENE__COMPONENT_TYPE_HPP

/// @brief Literals representing types of available components.
enum class ComponentType
{
    Unknown,
    /// @brief Literals representing types of available components.
    Mesh,
    Light,
    Camera,
    Script,
    InputProcessingScript
};

// New literals representing new classes derived from Component must be added here

#endif//CORE__SCENE__COMPONENT_TYPE_HPP