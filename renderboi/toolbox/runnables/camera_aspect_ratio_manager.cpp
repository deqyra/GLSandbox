#include "camera_aspect_ratio_manager.hpp"

CameraAspectRatioManager::CameraAspectRatioManager(CameraPtr camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    _camera->setAspectRatio((float)width / (float)height);
}
