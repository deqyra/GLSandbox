#ifndef CORE__SCENE__COMPONENT_TYPE_HPP
#define CORE__SCENE__COMPONENT_TYPE_HPP

/// @brief Literals describing types of available components.
enum class ComponentType
{
    Unknown,
    /// @brief Literals representing a MeshComponent.
    Mesh,
    /// @brief Literals representing a LightComponent.
    Light,
    /// @brief Literals representing a CameraComponent.
    Camera,
    /// @brief Literals representing a ScriptComponent.
    Script,
    /// @brief Literals representing a InputProcessingScriptComponent.
    InputProcessingScript
};

// New literals representing new classes derived from Component must be added here

#endif//CORE__SCENE__COMPONENT_TYPE_HPP