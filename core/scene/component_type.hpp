#ifndef CORE__SCENE__COMPONENT_TYPE_HPP
#define CORE__SCENE__COMPONENT_TYPE_HPP

// Literals representing types of available components
enum class ComponentType
{
    Unknown,
    Mesh,
    Light,
    Camera,
    Script,
    InputProcessingScript
};

// New litterals representing new classes derived from Component must be added here

#endif//CORE__SCENE__COMPONENT_TYPE_HPP