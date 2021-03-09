#ifndef RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_MOVEMENT_SCRIPT_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_MOVEMENT_SCRIPT_HPP

#include <string>

#include <glm/vec2.hpp>

#include <renderboi/core/interfaces/basis_provider.hpp>
#include <renderboi/window/gamepad/gamepad.hpp>
#include <renderboi/window/gamepad/gamepad_input_processor.hpp>

#include "../script.hpp"

namespace Renderboi
{

/// @brief Provides bindings to move an entity using a gamepad.
class GamepadMovementScript : public Script,
                              public GamepadInputProcessor
{
private:
    GamepadMovementScript(const GamepadMovementScript& other) = delete;
    GamepadMovementScript& operator=(const GamepadMovementScript& other) = delete;

    /// @brief Pointer to the entity which will provide directional vectors,
    /// used to move in the correct directions.
    BasisProviderPtr _basisProvider;

    /// @brief Vector telling the direction in which to go.
    glm::vec2 _direction;

    /// @brief Speed of the movement induced by keypresses.
    float _moveSpeed;

    /// @brief Speed multiplier when sprinting.
    float _sprintMultiplier;
    
    /// @brief Sprint flag.
    bool _sprint;
    
public:
    /// @brief The default move speed (movement with left thumbstick).
    static constexpr float DefaultMoveSpeed = 4.f;
    
    /// @brief The default sprint multiplier.
    static constexpr float DefaultSprintMultiplier = 1.5f;

    /// @param basisProvider Entity which will provide directional vectors.
    /// @param speed The speed of movement induced by gamepad input.
    /// @param sprintMultiplier The multiplier to be used when sprinting.
    ///
    /// @exception If the provided BasisProvider pointer is null, the 
    /// function will throw a std::runtime_error.
    GamepadMovementScript(
        const BasisProviderPtr basisProvider,
        const float speed = DefaultMoveSpeed,
        const float sprintMultiplier = DefaultSprintMultiplier
    );

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
    GamepadMovementScript* clone() const override;

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for a gamepad button event.
    ///
    /// @param button Literal describing which key triggered the event.
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event.
    virtual void processButton(const GamepadPtr gamepad, const Window::Input::Gamepad::Button button, const Window::Input::Action action) override;

    /// @brief Callback for a gamepad axis event.
    ///
    /// @param axis Literal describing which axis triggered the event.
    /// @param value Value at which the axis was polled.
    virtual void processAxis(const GamepadPtr gamepad, const Window::Input::Gamepad::Axis axis, const float value) override;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__GAMEPAD_MOVEMENT_SCRIPT_HPP