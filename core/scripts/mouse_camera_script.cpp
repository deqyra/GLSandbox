#include "mouse_camera_script.hpp"

#include "../scene/components/camera_component.hpp"

#include "../frame_of_reference.hpp"
using Ref = FrameOfReference;

MouseCameraScript::MouseCameraScript(float sensitivity) :
    _lookSensitivity(sensitivity),
    _lastMouseX(0),
    _lastMouseY(0),
    _mouseWasUpdatedOnce(false)
{

}

void MouseCameraScript::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{
    // If the mouse was never updated before, record its position and skip this update
    if (!_mouseWasUpdatedOnce)
    {
        _lastMouseX = (float)xpos;
        _lastMouseY = (float)ypos;
        _mouseWasUpdatedOnce = true;
        return;
    }
    // This is to avoid a huge mouse jump upon entering the window with the mouse

    // Compute offsets
    float yawOffset = (float)(xpos - _lastMouseX) * _lookSensitivity;
    // Y offset reversed since y-coordinates range from bottom to top
    float pitchOffset = (float)(_lastMouseY - ypos) * _lookSensitivity;

    _camera->processRotation(yawOffset, pitchOffset);
    // Record mouse position
    _lastMouseX = (float)xpos;
    _lastMouseY = (float)ypos;
}

void MouseCameraScript::setSceneObject(SceneObjectPtr sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "MouseCameraScript (script ID " + std::to_string(Script::id) + ", input processor ID " + std::to_string(InputProcessor::id) + ") was provided a null scene object pointer.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve camera component
    std::shared_ptr<CameraComponent> cameraComponent = sceneObject->getComponent<CameraComponent>();
    if (!cameraComponent)
    {
        std::string s = "MouseCameraScript (script ID " + std::to_string(Script::id) + ", input processor ID " + std::to_string(InputProcessor::id) + ") was attached to a scene object with no camera component.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
    _camera = cameraComponent->getCamera();
}

MouseCameraScript* MouseCameraScript::clone()
{
    return new MouseCameraScript(_lookSensitivity);
}