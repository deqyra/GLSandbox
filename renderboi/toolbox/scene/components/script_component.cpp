#include "script_component.hpp"

#include "../scene.hpp"

ScriptComponent::ScriptComponent(const SceneObjectPtr sceneObject, const ScriptPtr script) :
    Component(ComponentType::Script, sceneObject),
    _script(script)
{
    if (!script)
    {
        throw std::runtime_error("ScriptComponent: cannot construct from null script pointer.");
    }

    _script->setSceneObject(sceneObject);
    registerScript();
}

ScriptComponent::~ScriptComponent()
{
    releaseSceneObject();
}

ScriptPtr ScriptComponent::getScript() const
{
    return _script;
}

void ScriptComponent::setScript(const ScriptPtr script)
{
    if (!script)
    {
        throw std::runtime_error("ScriptComponent: cannot set script pointer to null.");
    }

    _script = script;
}

ScriptComponent* ScriptComponent::clone(const SceneObjectPtr newParent) const
{
    ScriptPtr clonedScript = ScriptPtr(_script->clone());
    ScriptComponent* clonedComponent = new ScriptComponent(newParent, clonedScript);
    // The parent scene object needs to be registered in the component, which is to be done by the caller
    return clonedComponent;
}

void ScriptComponent::registerScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject;
    if (!sceneObject) return;

    // Subscribe script to updates
    ScenePtr scene = sceneObject->getScene();
    if (!scene) return;
    
    scene->registerScript(_script);
}

void ScriptComponent::detachScript()
{
    // Get a pointer to the scene hosting the parent object
    if (!_sceneObject) return;

    // If applicable, detach script from scene updates
    ScenePtr scene = _sceneObject->getScene();
    if (!scene) return;

    scene->detachScript(_script->id);
}

void ScriptComponent::releaseSceneObject()
{
    detachScript();
    _sceneObject.reset();
    _script->setSceneObject(nullptr);
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

template<>
bool Component::multipleInstancesAllowed<ScriptComponent>()
{
    return true;
}
