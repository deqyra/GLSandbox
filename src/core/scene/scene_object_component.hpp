#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "scene_object_component_type.hpp"

#include <string>
#include <memory>

class SceneObject;

class SceneObjectComponent
{
    using ObjPtr = std::weak_ptr<SceneObject>;
    using CompType = SceneObjectComponentType;
    using CompPtr = std::shared_ptr<SceneObjectComponent>;

    private:
        SceneObjectComponent() = delete;

    protected:
        SceneObjectComponent(SceneObjectComponentType type, ObjPtr sceneObject = ObjPtr());

    public:
        virtual ~SceneObjectComponent();

        const SceneObjectComponentType type;
        ObjPtr sceneObject;

        template<class T>
        static CompType componentType();

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