#include "input_processing_script.hpp"

InputProcessingScript::InputProcessingScript()
{

}

InputProcessingScript::~InputProcessingScript()
{
    removeInputProcessor();
}

void InputProcessingScript::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{

}

void InputProcessingScript::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void InputProcessingScript::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void InputProcessingScript::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void InputProcessingScript::update(float timeElapsed)
{

}

void InputProcessingScript::setSceneObject(SceneObjectWPtr sceneObject)
{
    removeInputProcessor();
    Script::setSceneObject(sceneObject);
    registerInputProcessor();
}

void InputProcessingScript::registerInputProcessor()
{
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    InputProcessorPtr ptr = std::static_pointer_cast<InputProcessor>(this->shared_from_this());
    scene->registerInputProcessor(ptr);
}

void InputProcessingScript::removeInputProcessor()
{
    SceneObjectPtr sceneObject = _sceneObject.lock();
    ScenePtr scene = sceneObject->getScene().lock();
    scene->removeInputProcessor(id);
}