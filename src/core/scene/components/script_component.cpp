#include "script_component.hpp"

#include "../../scene/scene.hpp"

ScriptComponent::ScriptComponent(ScriptPtr script) :
    Component(ComponentType::Script),
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
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    // Subscribe script to updates
    scene->registerScript(_script);
}

void ScriptComponent::removeScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    // If applicable, detach script from scene updates
    ScenePtr scene = sceneObject->getScene().lock();
    scene->removeScript(_script->id);
}

template<>
ComponentType Component::componentType<ScriptComponent>()
{
    return ComponentType::Script;
}

template<>
std::string Component::componentTypeString<ScriptComponent>()
{
    return "Script";
}
