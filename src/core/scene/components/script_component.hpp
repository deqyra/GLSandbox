#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../script.hpp"

#include <string>

class ScriptComponent : public SceneObjectComponent
{
    private:
        ScriptPtr _script;

    public:
        ScriptComponent(ScriptPtr script);
        virtual ~ScriptComponent();

        virtual void setSceneObject(SceneObjectWPtr sceneObject);
};

template<>
SceneObjectComponentType SceneObjectComponent::componentType<ScriptComponent>();

template<>
std::string SceneObjectComponent::componentTypeString<ScriptComponent>();

#endif//SCRIPT_COMPONENT_HPP