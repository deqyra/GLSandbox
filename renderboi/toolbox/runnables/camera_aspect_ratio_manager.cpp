#include "camera_aspect_ratio_manager.hpp"

CameraAspectRatioManager::CameraAspectRatioManager(const CameraPtr camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(const GLWindowPtr window, const int width, const int height)
{
    _camera->setAspectRatio((float)width / (float)height);
}
