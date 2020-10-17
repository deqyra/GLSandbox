#ifndef CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP
#define CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP

#include "../camera.hpp"
#include "../scene/input_processing_script.hpp"

/// @brief Provides event callbacks to manage a camera as in a FPS game.
class FPSCameraScript : public InputProcessingScript
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

        FPSCameraScript(const FPSCameraScript& other) = delete;
        FPSCameraScript& operator=(const FPSCameraScript& other) = delete;

        /// @brief Pointer to the camera controlled by the script.
        CameraPtr _camera;

        /// @brief Camera move speed (WASD).
        float _moveSpeed;

        /// @brief Camera sensitivity (mouse).
        float _lookSensitivity;

        /// @brief Speed multiplier when sprinting.
        float _sprintMultiplier;
        
        /// @brief Directional movement flags.
        bool _movement[4];
        /// @brief Sprint flag.
        bool _sprint;

        /// @brief Last X position the mouse was recorded at.
        float _lastMouseX;

        /// @brief Last Y position the mouse was recorded at.
        float _lastMouseY;

        /// @brief Whether the mouse was updated once.
        bool _mouseWasUpdatedOnce;
        
    public:
        /// @brief The default move speed (movement with WASD keys).
        static constexpr float DefaultMoveSpeed = 4.f;
        
        /// @brief The default mouse sensitivity.
        static constexpr float DefaultLookSensitivity = 0.1f;

        /// @brief The default sprint multiplier.
        static constexpr float DefaultSprintMultiplier = 1.5f;

        /// @param speed The speed of movement induced by WASD keys.
        /// @param sensitivity The amplitude of the rotation induced by mouse movement.
        /// @param sprintMultiplier The multiplier to be used when sprinting.
        FPSCameraScript(float speed = DefaultMoveSpeed, float sensitivity = DefaultLookSensitivity, float sprintMultiplier = DefaultSprintMultiplier);

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
        virtual void processKeyboard(Window::GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(Window::GLWindowPtr window, double xpos, double ypos);

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(float timeElapsed);

        /// @brief Set the scene object which the camera script is attached to.
        /// Will also attempt to retrieve a camera from the scene object.
        ///
        /// @param sceneObject Weak pointer to the scene object the script 
        /// should be attached to.
        ///
        /// @exception If the provided pointer is null, or if the scene object
        /// pointed at has no camera, this function will throw a 
        /// std::runtime_error.
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        /// @brief Get a raw pointer to a new camera script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual FPSCameraScript* clone();
};

#endif//CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP