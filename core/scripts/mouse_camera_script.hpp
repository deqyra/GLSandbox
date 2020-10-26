#ifndef CORE__SCRIPTS__MOUSE_CAMERA_SCRIPT_HPP
#define CORE__SCRIPTS__MOUSE_CAMERA_SCRIPT_HPP

#include "../camera.hpp"
#include "../scene/input_processing_script.hpp"

/// @brief Provides event callbacks to manage a camera as in a FPS game.
class MouseCameraScript : public InputProcessingScript
{
    private:
        MouseCameraScript(const MouseCameraScript& other) = delete;
        MouseCameraScript& operator=(const MouseCameraScript& other) = delete;

        /// @brief Pointer to the camera controlled by the script.
        CameraPtr _camera;

        /// @brief Camera sensitivity (mouse).
        float _lookSensitivity;

        /// @brief Last X position the mouse was recorded at.
        float _lastMouseX;

        /// @brief Last Y position the mouse was recorded at.
        float _lastMouseY;

        /// @brief Whether the mouse was updated once.
        bool _mouseWasUpdatedOnce;

        /// @brief Update the rotation of the parent scene object to fit 
        /// recorded offsets in yaw and pitch.
        ///
        /// @param yawOffset How much the camera was rotated around Y in degrees.
        /// @param pitchOffset How much the camera was rotated around X in degrees.
        void processRotation(float yawOffset, float pitchOffset);
        
    public:
        /// @brief The default mouse sensitivity.
        static constexpr float DefaultLookSensitivity = 0.1f;

        /// @param sensitivity The amplitude of the rotation induced by mouse movement.
        MouseCameraScript(float sensitivity = DefaultLookSensitivity);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        /// @brief Set the scene object which the camera script is attached to.
        /// Will also attempt to retrieve a camera from the scene object.
        ///
        /// @param sceneObject Pointer to the scene object the script should be 
        /// attached to.
        ///
        /// @exception If the provided pointer is null, this function will throw
        /// a std::runtime_error.
        virtual void setSceneObject(SceneObjectPtr sceneObject);

        /// @brief Get a raw pointer to a new camera script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual MouseCameraScript* clone();
};

#endif//CORE__SCRIPTS__MOUSE_CAMERA_SCRIPT_HPP