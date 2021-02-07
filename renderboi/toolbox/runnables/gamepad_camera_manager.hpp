#ifndef RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_CAMERA_MANAGER_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_CAMERA_MANAGER_HPP

#include <glm/vec2.hpp>

#include <renderboi/core/camera.hpp>
#include <renderboi/window/gamepad/gamepad_input_processor.hpp>
#include <renderboi/window/gamepad/gamepad.hpp>

#include "../script.hpp"

namespace Renderboi
{

/// @brief Provides event callbacks to manage a camera as in a FPS game. This 
/// script must be attached to an object which has a camera component, otherwise
/// an std::runtime_error will be thrown upon attaching.
class GamepadCameraManager : public GamepadInputProcessor,
                             public Script
{
private:
    GamepadCameraManager(const GamepadCameraManager& other) = delete;
    GamepadCameraManager& operator=(const GamepadCameraManager& other) = delete;

    /// @brief Pointer to the camera controlled by the script.
    const CameraPtr _camera;

    /// @brief Camera sensitivity (right thumbstick).
    float _lookSensitivity;

    /// @brief Direction in which the camera look is requested to move in.
    glm::vec2 _direction;

public:
    /// @brief The default thumbstick sensitivity.
    static constexpr float DefaultLookSensitivity = 0.1f;

    /// @param camera Pointer to the camera whose orientation to manage.
    /// @param sensitivity The amplitude of the rotation induced by the right 
    /// thumbstick.
    GamepadCameraManager(const CameraPtr camera, const float sensitivity = DefaultLookSensitivity);

    //////////////////////////////////////
    ///                                ///
    /// Methods overridden from Script ///
    ///                                ///
    //////////////////////////////////////

    /// @brief Make the script run and do its things.
    ///
    /// @param timeElapsed How much time passed (in seconds) since the last
    /// update.
    void update(float timeElapsed) override;

    /// @brief Set the scene object which the camera script is attached to.
    /// Will also attempt to retrieve a camera from the scene object.
    ///
    /// @param sceneObject Pointer to the scene object the script should be
    /// attached to.
    ///
    /// @exception If the provided pointer is null, this function will throw
    /// a std::runtime_error.
    void setSceneObject(const SceneObjectPtr sceneObject) override;

    /// @brief Get a raw pointer to a new keyboard script instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @return A raw pointer to the script instance cloned from this one.
    GamepadCameraManager* clone() const override;

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for a gamepad axis event.
    ///
    /// @param axis Literal describing which axis triggered the event.
    /// @param value Value at which the axis was polled.
    virtual void processAxis(const GamepadPtr gamepad, const Window::Input::Gamepad::Axis axis, const float value) override;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_CAMERA_MANAGER_HPP