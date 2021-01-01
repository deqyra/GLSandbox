#include "camera_aspect_ratio_manager.hpp"

CameraAspectRatioManager::CameraAspectRatioManager(const CameraPtr camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height)
{
    _camera->setAspectRatio((float)width / (float)height);
}
