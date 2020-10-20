#include "input_processing_script_component.hpp"

#include "../../scene/scene.hpp"

InputProcessingScriptComponent::InputProcessingScriptComponent(InputProcessingScriptPtr script) :
    Component(ComponentType::InputProcessingScript),
    _script(script)
{

}

InputProcessingScriptComponent::~InputProcessingScriptComponent()
{

}

InputProcessingScriptPtr InputProcessingScriptComponent::getScript()
{
    return _script;
}

void InputProcessingScriptComponent::setScript(InputProcessingScriptPtr script)
{
    if (!script)
    {
        std::string s = "NONE";
        std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
        if (sceneObject) s = std::to_string(sceneObject->id);

        s = "InputScriptComponent (on scene object with ID " + s + ") was passed a null script pointer.";
        throw std::runtime_error(s.c_str());
    }

    _script = script;
}

void InputProcessingScriptComponent::setSceneObject(SceneObjectWPtr sceneObject)
{
    detachScript();
    _sceneObject = sceneObject;
    _script->setSceneObject(sceneObject);
    registerScript();
}

InputProcessingScriptComponent* InputProcessingScriptComponent::clone()
{
    InputProcessingScriptPtr clonedScript = InputProcessingScriptPtr(_script->clone());
    InputProcessingScriptComponent* clonedComponent = new InputProcessingScriptComponent(clonedScript);
    // The parent scene object needs to be registered in the cloned component, which is to be done by the caller.
    // In doing so, the parent scene object will also be registered in the inner script.
    return clonedComponent;
}

void InputProcessingScriptComponent::registerScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    // Subscribe script to updates
    ScenePtr scene = sceneObject->getScene();
    if (scene == nullptr) return;
    
    scene->registerInputProcessingScript(_script);
}

void InputProcessingScriptComponent::detachScript()
{
    // Get a pointer to the scene hosting the parent object
    SceneObjectPtr sceneObject = _sceneObject.lock();
    if (sceneObject == nullptr) return;

    // If applicable, detach script from scene updates
    ScenePtr scene = sceneObject->getScene();
    if (scene == nullptr) return;
    
    scene->detachInputProcessingScript(_script);
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
