#ifndef RENDERBOI__TOOLBOX__SCRIPTS__CAMERA_ASPECT_RATIO_SCRIPT_HPP
#define RENDERBOI__TOOLBOX__SCRIPTS__CAMERA_ASPECT_RATIO_SCRIPT_HPP

#include <renderboi/core/camera.hpp>

#include "../input_processing_script.hpp"

/// @brief Provides event callbacks to manage the aspect ratio of a camera as 
/// the framebuffer size changes. This script must be attached to an object 
/// which has a camera component, otherwise an std::runtime_error will be thrown
/// upon attaching.
class CameraAspectRatioScript : public InputProcessingScript
{
    private:
        CameraAspectRatioScript(const CameraAspectRatioScript& other) = delete;
        CameraAspectRatioScript& operator=(const CameraAspectRatioScript& other) = delete;

        /// @brief Pointer to the camera controlled by the script.
        CameraPtr _camera;
        
    public:
        CameraAspectRatioScript();

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);

        /// @brief Set the scene object which the camera script is attached to.
        /// Will also attempt to retrieve a camera from the scene object.
        ///
        /// @param sceneObject Pointer to the scene object the script should be 
        /// attached to.
        ///
        /// @exception If the provided pointer is null, or if the scene object 
        /// has no camera component, this function will throw a 
        /// std::runtime_error.
        virtual void setSceneObject(SceneObjectPtr sceneObject);

        /// @brief Get a raw pointer to a new camera script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual CameraAspectRatioScript* clone();
};

#endif//RENDERBOI__TOOLBOX__SCRIPTS__CAMERA_ASPECT_RATIO_SCRIPT_HPP