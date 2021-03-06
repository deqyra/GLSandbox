#ifndef RENDERBOI__TOOLBOX__RUNNABLES__KEYBOARD_MOVEMENT_SCRIPT_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__KEYBOARD_MOVEMENT_SCRIPT_HPP

#include <string>

#include <renderboi/core/interfaces/basis_provider.hpp>

#include "../script.hpp"
#include "../controls/control_scheme_manager.hpp"
#include "../interfaces/action_event_receiver.hpp"
#include "../interfaces/default_control_scheme_provider.hpp"

namespace Renderboi
{

/// @brief Litterals describing the actions which can be performed by the
/// KeyboardMovementScript.
enum class KeyboardMovementAction
{
    Forward,
    Backward,
    Left,
    Right,
    Sprint
};

/// @brief Provides bindings to move an entity using the keyboard.
class KeyboardMovementScript : public Script,
                               public ActionEventReceiver<KeyboardMovementAction>,
                               public DefaultControlSchemeProvider<KeyboardMovementAction>
{
private:
    /// @brief Index of a bool array which flags a forward keypress.
    static constexpr unsigned int _IndexForward = 0;
    
    /// @brief Index of a bool array which flags a backward keypress.
    static constexpr unsigned int _IndexBackward = 1;
    
    /// @brief Index of a bool array which flags a left keypress.
    static constexpr unsigned int _IndexLeft = 2;
    
    /// @brief Index of a bool array which flags a right keypress.
    static constexpr unsigned int _IndexRight = 3;

    KeyboardMovementScript(const KeyboardMovementScript& other) = delete;
    KeyboardMovementScript& operator=(const KeyboardMovementScript& other) = delete;

    /// @brief Pointer to the entity which will provide directional vectors,
    /// used to move in the correct directions.
    BasisProviderPtr _basisProvider;

    /// @brief Speed of the movement induced by keypresses.
    float _moveSpeed;

    /// @brief Speed multiplier when sprinting.
    float _sprintMultiplier;
    
    /// @brief Directional movement flags.
    bool _movementFlags[4];

    /// @brief Sprint flag.
    bool _sprint;

    /// @brief Cancel directional flags when raised in opposite directions.
    void cancelOppositeDirections();
    
public:
    using ActionType = KeyboardMovementAction;

    /// @brief The default move speed (movement with WASD keys).
    static constexpr float DefaultMoveSpeed = 4.f;
    
    /// @brief The default sprint multiplier.
    static constexpr float DefaultSprintMultiplier = 1.5f;

    /// @param basisProvider Entity which will provide directional vectors.
    /// @param speed The speed of movement induced by keypresses.
    /// @param sprintMultiplier The multiplier to be used when sprinting.
    ///
    /// @exception If the provided BasisProvider pointer is null, the 
    /// function will throw a std::runtime_error.
    KeyboardMovementScript(
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
    KeyboardMovementScript* clone() const override;

    ///////////////////////////////////////////////////////////////////////////
    ///                                                                     ///
    /// Methods overridden from ActionEventReceiver<KeyboardMovementAction> ///
    ///                                                                     ///
    ///////////////////////////////////////////////////////////////////////////

    /// @brief Start the processing for an action.
    ///
    /// @param action Object describing the action to start processing.
    void triggerAction(const GLWindowPtr window, const KeyboardMovementAction& action) override;

    /// @brief Stop the processing for an action.
    ///
    /// @param action Object describing the action to stop processing.
    void stopAction(const GLWindowPtr window, const KeyboardMovementAction& action) override;

    ////////////////////////////////////////////////////////////////////////////////////
    ///                                                                              ///
    /// Methods overridden from DefaultControlSchemeProvider<KeyboardMovementAction> ///
    ///                                                                              ///
    ////////////////////////////////////////////////////////////////////////////////////

    /// @brief Get the default control scheme for the keyboard movement
    /// script.
    ///
    /// @return The default control scheme for the keyboard movement script.
    ControlSchemeManagerPtr<KeyboardMovementAction> getDefaultControlScheme() const override;
};

std::string to_string(const KeyboardMovementAction& action);

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__KEYBOARD_MOVEMENT_SCRIPT_HPP