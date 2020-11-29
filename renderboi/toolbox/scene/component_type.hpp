#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENT_TYPE_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENT_TYPE_HPP

/// @brief Literals describing types of available components.
enum class ComponentType
{
    Unknown,
    /// @brief Literal representing a MeshComponent.
    Mesh,
    /// @brief Literal representing a LightComponent.
    Light,
    /// @brief Literal representing a CameraComponent.
    Camera,
    /// @brief Literal representing a ScriptComponent.
    Script,
    /// @brief Literal representing a InputProcessingScriptComponent.
    InputProcessingScript
};

// New literals representing new classes derived from Component must be added here

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENT_TYPE_HPP