#ifndef CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../camera.hpp"

#include <string>

/// @brief Component allowing to attach a camera to a scene object.
class CameraComponent : public Component
{
    private:
        CameraComponent(CameraComponent& other) = delete;
        CameraComponent& operator=(const CameraComponent& other) = delete;

        /// @brief Pointer to the camera used by the component.
        CameraPtr _camera;

    public:
        /// @param camera Camera for the component to use.
        CameraComponent(CameraPtr camera);

        virtual ~CameraComponent();

        /// @brief Get a pointer to the camera used by the component.
        ///
        /// @return A pointer to the camera used by the component.
        CameraPtr getCamera();

        /// @brief Set the camera used by the component.
        ///
        /// @param camera Pointer to the camera the component should use.
        ///
        /// @exception If the passed camera pointer is null, the function will
        /// throw a std::runtime_error.
        void setCamera(CameraPtr camera);

        /// @brief Get the view matrix corresponding to the position and facing
        /// of the camera.
        ///
        /// @return The view matrix provided by the camera.
        virtual glm::mat4 getViewMatrix();

        /// @brief Get the view space coordinates of a position given in world 
        /// space coordinates.
        ///
        /// @param worldPosition The position whose coordinates to transform.
        ///
        /// @return View space coordinates of the provided world position.
        virtual glm::vec3 worldPositionToViewSpace(glm::vec3 worldPosition);

        /// @brief Get the projection matrix of the camera.
        ///
        /// @return The projection matrix provided by the camera.
        virtual glm::mat4 getProjectionMatrix();

        /// @brief Get the view-projection matrix of the camera.
        ///
        /// @return The view-projection matrix provided by the camera.
        virtual glm::mat4 getViewProjectionMatrix();

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual CameraComponent* clone();
};

template<>
ComponentType Component::componentType<CameraComponent>();

template<>
std::string Component::componentTypeString<CameraComponent>();

template<>
bool Component::multipleInstancesAllowed<CameraComponent>();

#endif//CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP