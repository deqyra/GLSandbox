#ifndef CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../camera.hpp"

#include <string>

// Component derived class representing a camera fixed to a scene object
class CameraComponent : public Component
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Components are meant to be used only through pointers
        CameraComponent(CameraComponent& other) = delete;
        CameraComponent& operator=(const CameraComponent& other) = delete;

        // Pointer to the camera used by the component
        CameraPtr _camera;

        // Get the direction "upwards" locally to the parent scene object
        glm::vec3 getParentUp();

    public:
        CameraComponent(CameraPtr camera);
        virtual ~CameraComponent();

        // Get a pointer to the camera used by the component
        CameraPtr getCamera();
        // Set the camera pointer used by the component
        void setCamera(CameraPtr camera);

        // Get the view matrix corresponding to the position and facing of the camera
        virtual glm::mat4 getViewMatrix();
        // Given the current state of the camera, transform a position frow world space into camera space
        virtual glm::vec3 worldPositionToViewSpace(glm::vec3 worldPosition);
        // Get the projection matrix corresponding to the camera
        virtual glm::mat4 getProjectionMatrix();
        // Get the VP matrix corresponding to the camera
        virtual glm::mat4 getViewProjectionMatrix();

        // Get a raw pointer to a new CameraComponent instance cloned from this. The inner camera is cloned as well. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual CameraComponent* clone();
};

template<>
ComponentType Component::componentType<CameraComponent>();

template<>
std::string Component::componentTypeString<CameraComponent>();

#endif//CORE__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP