#ifndef CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP
#define CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP

#include "../camera.hpp"
#include "../scene/input_processing_script.hpp"

// Provide event callbacks to manage a camera as in a FPS game
class FPSCameraScript : public InputProcessingScript
{
    private:
        static constexpr unsigned int IndexForward = 0;
        static constexpr unsigned int IndexBackward = 1;
        static constexpr unsigned int IndexLeft = 2;
        static constexpr unsigned int IndexRight = 3;

        // Disallow copy-constructor and copy-assignment operator as Scripts are meant to be used only through pointers
        FPSCameraScript(const FPSCameraScript& other) = delete;
        FPSCameraScript& operator=(const FPSCameraScript& other) = delete;

        // Pointer to the camera controlled by the script
        CameraPtr _camera;

        // Camera move speed (WASD)
        float _moveSpeed;
        // Camera sensitivity (mouse)
        float _lookSensitivity;
        // Speed multiplier when sprinting
        float _sprintMultiplier;

        // Movement flags
        bool _movement[4];
        // Sprint flag
        bool _sprint;

        // Last X position the mouse was recorded at
        float _lastMouseX;
        // Last Y position the mouse was recorded at
        float _lastMouseY;
        // Whether the mouse was updated once
        bool _mouseWasUpdatedOnce;

    public:
        static constexpr float DefaultMoveSpeed = 4.f;
        static constexpr float DefaultLookSensitivity = 0.1f;
        static constexpr float DefaultSprintMultiplier = 1.5f;

        FPSCameraScript(float speed = DefaultMoveSpeed, float sensitivity = DefaultLookSensitivity, float sprintMultiplier = DefaultSprintMultiplier);

        // To be called upon a keyboard event; handles WASD keys and moves camera
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event; handles mouse position shift and rotates camera
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        // To be called upon a frame update; does nothing
        virtual void update(float timeElapsed);

        // Set the scene object reference which the input processing script is attached to, retrieve camera from SceneObject
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        // Get a raw pointer to a new FPSCameraScript instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual FPSCameraScript* clone();
};

#endif//CORE__SCRIPTS__FPS_CAMERA_SCRIPT_HPP