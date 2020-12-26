#include "mouse_camera_manager.hpp"

#include <renderboi/core/frame_of_reference.hpp>
using Ref = FrameOfReference;

MouseCameraManager::MouseCameraManager(CameraPtr camera, float sensitivity) :
    _camera(camera),
    _lookSensitivity(sensitivity),
    _lastMouseX(0),
    _lastMouseY(0),
    _mouseWasUpdatedOnce(false)
{

}

void MouseCameraManager::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
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
