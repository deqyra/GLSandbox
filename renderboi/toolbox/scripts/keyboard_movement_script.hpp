#ifndef RENDERBOI__TOOLBOX__SCRIPTS__KEYBOARD_MOVEMENT_SCRIPT_HPP
#define RENDERBOI__TOOLBOX__SCRIPTS__KEYBOARD_MOVEMENT_SCRIPT_HPP

#include <renderboi/core/interfaces/basis_provider.hpp>

#include "../input_processing_script.hpp"

/// @brief Provides event callbacks to manage a camera as in a FPS game.
class KeyboardMovementScript : public InputProcessingScript
{
    private:
        /// @brief Index of a bool array which flags a forward keypress.
        static constexpr unsigned int IndexForward = 0;
        
        /// @brief Index of a bool array which flags a backward keypress.
        static constexpr unsigned int IndexBackward = 1;
        
        /// @brief Index of a bool array which flags a left keypress.
        static constexpr unsigned int IndexLeft = 2;
        
        /// @brief Index of a bool array which flags a right keypress.
        static constexpr unsigned int IndexRight = 3;

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
        bool _movement[4];

        /// @brief Sprint flag.
        bool _sprint;
        
    public:
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
        KeyboardMovementScript(BasisProviderPtr basisProvider, float speed = DefaultMoveSpeed, float sprintMultiplier = DefaultSprintMultiplier);

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(float timeElapsed);

        /// @brief Set the scene object which the camera script is attached to.
        /// Will also attempt to retrieve a camera from the scene object.
        ///
        /// @param sceneObject Pointer to the scene object the script should be
        /// attached to.
        ///
        /// @exception If the provided pointer is null, this function will throw
        /// a std::runtime_error.
        virtual void setSceneObject(SceneObjectPtr sceneObject);

        /// @brief Get a raw pointer to a new keyboard script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual KeyboardMovementScript* clone();
};


#endif//RENDERBOI__TOOLBOX__SCRIPTS__KEYBOARD_MOVEMENT_SCRIPT_HPP