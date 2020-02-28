#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "scene_object_component_type.hpp"

#include <string>

class SceneObjectComponent
{
    protected:
        SceneObjectComponent(SceneObjectComponentType type);

    public:
        const SceneObjectComponentType type;

        template<class T>
        static SceneObjectComponentType componentType();

        template<class T>
        static std::string componentTypeString();
};

template<class T>
SceneObjectComponentType SceneObjectComponent::componentType()
{
    return SceneObjectComponentType::Unknown;
}

template<class T>
std::string SceneObjectComponent::componentTypeString()
{
    return "Unknown";
}

#endif//COMPONENT_HPP