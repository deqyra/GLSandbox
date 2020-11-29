#include "input_processing_script_component.hpp"

#include "../scene.hpp"

InputProcessingScriptComponent::InputProcessingScriptComponent(SceneObjectPtr sceneObject, InputProcessingScriptPtr script) :
    Component(ComponentType::InputProcessingScript, sceneObject),
    _script(script)
{
    if (!script)
    {
        throw std::runtime_error("InputProcessingScriptComponent: cannot construct from null script pointer.");
    }

    _script->setSceneObject(sceneObject);
    registerScript();
}

InputProcessingScriptComponent::~InputProcessingScriptComponent()
{
    releaseSceneObject();
}

InputProcessingScriptPtr InputProcessingScriptComponent::getScript()
{
    return _script;
}

void InputProcessingScriptComponent::setScript(InputProcessingScriptPtr script)
{
    if (!script)
    {
        throw std::runtime_error("InputProcessingScriptComponent: cannot set script to null pointer.");
    }

    _script = script;
}

InputProcessingScriptComponent* InputProcessingScriptComponent::clone(SceneObjectPtr newParent)
{
    InputProcessingScriptPtr clonedScript = InputProcessingScriptPtr(_script->clone());
    InputProcessingScriptComponent* clonedComponent = new InputProcessingScriptComponent(newParent, clonedScript);

    return clonedComponent;
}

void InputProcessingScriptComponent::registerScript()
{
    // Get a pointer to the scene hosting the parent object
    if (!_sceneObject) return;

    ScenePtr scene = _sceneObject->getScene();
    if (!scene) return;
    
    // Subscribe script to updates
    scene->registerInputProcessingScript(_script);
}

void InputProcessingScriptComponent::detachScript()
{
    // Get a pointer to the scene hosting the parent object
    if (!_sceneObject) return;

    // If applicable, detach script from scene updates
    ScenePtr scene = _sceneObject->getScene();
    if (!scene) return;
    
    scene->detachInputProcessingScript(_script);
}

void InputProcessingScriptComponent::releaseSceneObject()
{
    detachScript();
    _sceneObject.reset();
    _script->setSceneObject(nullptr);
}

template<>
ComponentType Component::componentType<InputProcessingScriptComponent>()
{
    return ComponentType::InputProcessingScript;
}

template<>
std::string Component::componentTypeString<InputProcessingScriptComponent>()
{
    return "InputProcessingScript";
}

template<>
bool Component::multipleInstancesAllowed<InputProcessingScriptComponent>()
{
    return true;
}
