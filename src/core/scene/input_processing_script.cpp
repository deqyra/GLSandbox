#include "input_processing_script.hpp"

InputProcessingScript::InputProcessingScript()
{

}

InputProcessingScript::~InputProcessingScript()
{
    removeInputProcessor();
}

void InputProcessingScript::setSceneObject(SceneObjectWPtr sceneObject)
{
    removeInputProcessor();

    _sceneObject = sceneObject;
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();

    InputProcessorWPtr ptr = std::static_pointer_cast<InputProcessor>(this->weak_from_this);
    scene->registerInputProcessor(ptr);
}

void InputProcessingScript::removeInputProcessor()
{
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    scene->removeInputProcessor(id);
}