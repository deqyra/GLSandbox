#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "scene_types_decl.hpp"
#include "scene_object_component_type.hpp"

#include <memory>
#include <string>

class SceneObjectComponent
{
    private:
        SceneObjectComponent() = delete;

    protected:
        SceneObjectComponent(SceneObjectComponentType type, SceneObjectWPtr sceneObject = SceneObjectWPtr());
        SceneObjectWPtr _sceneObject;

    public:
        virtual ~SceneObjectComponent();

        const SceneObjectComponentType type;

        virtual SceneObjectWPtr getSceneObject();
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

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