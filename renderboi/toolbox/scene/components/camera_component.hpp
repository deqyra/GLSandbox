#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP

#include <string>

#include <renderboi/core/camera.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

/// @brief Component allowing to attach a camera to a scene object.
class CameraComponent : public Component
{
    private:
        CameraComponent(CameraComponent& other) = delete;
        CameraComponent& operator=(const CameraComponent& other) = delete;

        /// @brief Pointer to the camera used by the component.
        CameraPtr _camera;

    public:
        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param camera Camera for the component to use.
        ///
        /// @exception If the provided scene object pointer is null, the 
        /// constructor will throw a std::runtime_error.
        CameraComponent(const SceneObjectPtr sceneObject, const CameraPtr camera);

        ~CameraComponent();

        /// @brief Get a pointer to the camera used by the component.
        ///
        /// @return A pointer to the camera used by the component.
        CameraPtr getCamera() const;

        /// @brief Set the camera used by the component.
        ///
        /// @param camera Pointer to the camera the component should use.
        ///
        /// @exception If the passed camera pointer is null, the function will
        /// throw a std::runtime_error.
        void setCamera(const CameraPtr camera);

        /// @brief Get the view matrix corresponding to the position and facing
        /// of the camera.
        ///
        /// @return The view matrix provided by the camera.
        glm::mat4 getViewMatrix() const;

        /// @brief Get the view space coordinates of a position given in world 
        /// space coordinates.
        ///
        /// @param worldPosition The position whose coordinates to transform.
        ///
        /// @return View space coordinates of the provided world position.
        glm::vec3 worldPositionToViewSpace(const glm::vec3 worldPosition) const;

        /// @brief Get the projection matrix of the camera.
        ///
        /// @return The projection matrix provided by the camera.
        glm::mat4 getProjectionMatrix() const;

        /// @brief Get the view-projection matrix of the camera.
        ///
        /// @return The view-projection matrix provided by the camera.
        glm::mat4 getViewProjectionMatrix() const;

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from Component ///
        ///                                   ///
        /////////////////////////////////////////

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @param newParent Pointer the scene object which will be parent to
        /// the cloned component instance.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        CameraComponent* clone(const SceneObjectPtr newParent) const;
};

template<>
ComponentType Component::componentType<CameraComponent>();

template<>
std::string Component::componentTypeString<CameraComponent>();

template<>
bool Component::multipleInstancesAllowed<CameraComponent>();

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP