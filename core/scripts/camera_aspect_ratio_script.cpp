#include "camera_aspect_ratio_script.hpp"

CameraAspectRatioScript::CameraAspectRatioScript()
{

}

void CameraAspectRatioScript::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    _camera->setAspectRatio((float)width / (float)height);
}

void CameraAspectRatioScript::setSceneObject(SceneObjectPtr sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "CameraAspectRatioScript: null scene object pointer was provided.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve camera component
    std::shared_ptr<CameraComponent> cameraComponent = sceneObject->getComponent<CameraComponent>();
    if (!cameraComponent)
    {
        std::string s = "CameraAspectRatioScript: attempt to attach to a scene object with no camera component.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
    _camera = cameraComponent->getCamera();
}

CameraAspectRatioScript* CameraAspectRatioScript::clone()
{
    return new CameraAspectRatioScript();
}