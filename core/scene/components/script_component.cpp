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

ScriptPtr ScriptComponent::getScript()
{
    return _script;
}

void ScriptComponent::setScript(ScriptPtr script)
{
    if (!script)
    {
        std::string s = "NONE";
        std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
        if (sceneObject) s = std::to_string(sceneObject->id);

        s = "ScriptComponent (on scene object with ID " + s + ") was passed a null script pointer.";
        throw std::runtime_error(s.c_str());
    }

    _script = script;
}

void ScriptComponent::setSceneObject(SceneObjectWPtr sceneObject)
{
    detachScript();
    _sceneObject = sceneObject;
    _script->setSceneObject(sceneObject);
    registerScript();
}

ScriptComponent* ScriptComponent::clone()
{
    ScriptPtr clonedScript = ScriptPtr(_script->clone());
    ScriptComponent* clonedComponent = new ScriptComponent(clonedScript);
    // The parent scene object needs to be registered in the component, which is to be done by the caller
    return clonedComponent;
}

void ScriptComponent::registerScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    // Subscribe script to updates
    ScenePtr scene = sceneObject->getScene().lock();
    if (scene == nullptr) return;
    
    scene->registerScript(_script);
}

void ScriptComponent::detachScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    // If applicable, detach script from scene updates
    ScenePtr scene = sceneObject->getScene().lock();
    scene->detachScript(_script->id);
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
