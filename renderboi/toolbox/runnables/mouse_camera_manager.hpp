#ifndef RENDERBOI__TOOLBOX__RUNNABLES__MOUSE_CAMERA_MANAGER_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__MOUSE_CAMERA_MANAGER_HPP

#include <renderboi/core/camera.hpp>

#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gl_window.hpp>

namespace Renderboi
{

/// @brief Provides event callbacks to manage a camera as in a FPS game. This 
/// script must be attached to an object which has a camera component, otherwise
/// an std::runtime_error will be thrown upon attaching.
class MouseCameraManager : public InputProcessor
{
private:
    MouseCameraManager(const MouseCameraManager& other) = delete;
    MouseCameraManager& operator=(const MouseCameraManager& other) = delete;

    /// @brief Pointer to the camera controlled by the script.
    const CameraPtr _camera;

    /// @brief Camera sensitivity (mouse).
    float _lookSensitivity;

    /// @brief Last X position the mouse was recorded at.
    float _lastMouseX;

    /// @brief Last Y position the mouse was recorded at.
    float _lastMouseY;

    /// @brief Whether the mouse was updated once.
    bool _mouseWasUpdatedOnce;

public:
    /// @brief The default mouse sensitivity.
    static constexpr float DefaultLookSensitivity = 0.1f;

    /// @param camera Pointer to the camera whose orientation to manage.
    /// @param sensitivity The amplitude of the rotation induced by mouse movement.
    MouseCameraManager(const CameraPtr camera, const float sensitivity = DefaultLookSensitivity);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a mouse cursor event.
    ///
    /// @param window Pointer to the GLWindow in which the event was
    /// triggered.
    /// @param xpos X coordinate of the new position of the mouse.
    /// @param ypos Y coordinate of the new position of the mouse.
    void processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos) override;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__MOUSE_CAMERA_MANAGER_HPP