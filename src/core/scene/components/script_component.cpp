#include "script_component.hpp"

using CompType = SceneObjectComponentType;

ScriptComponent::ScriptComponent(ScriptPtr script) :
    SceneObjectComponent(CompType::Script),
    _script(script)
{

}

ScriptComponent::~ScriptComponent()
{

}

void ScriptComponent::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
    _script->setSceneObject(sceneObject);
}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<ScriptComponent>()
{
    return SceneObjectComponentType::Script;
}

template<>
std::string SceneObjectComponent::componentTypeString<ScriptComponent>()
{
    return "Script";
}
