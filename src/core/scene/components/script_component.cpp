#include "script_component.hpp"

#include "../../scene.hpp"

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
    removeScript();
    _sceneObject = sceneObject;
    _script->setSceneObject(sceneObject);
    registerScript();
}

void ScriptComponent::registerScript()
{
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    scene->registerScript(_script);
}

void ScriptComponent::removeScript()
{
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    ScenePtr scene = sceneObject->getScene().lock();
    scene->removeInputProcessor(_script->id);
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
