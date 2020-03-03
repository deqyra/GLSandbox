#include "input_processing_script.hpp"

InputProcessingScript::InputProcessingScript()
{

}

InputProcessingScript::~InputProcessingScript()
{

}

void InputProcessingScript::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    InputProcessorWPtr ptr = std::static_pointer_cast<InputProcessor>(this->weak_from_this);
}
