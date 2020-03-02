#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../../camera.hpp"
#include "../../view_projection_provider.hpp"

#include <string>

class CameraComponent : public SceneObjectComponent, public ViewProjectionProvider
{
    public:
        CameraComponent(std::shared_ptr<Camera> camera);
        virtual ~CameraComponent();

        std::shared_ptr<Camera> camera;

        virtual glm::mat4 getViewMatrix();
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition);
        virtual glm::mat4 getProjectionMatrix();
        virtual glm::mat4 getViewProjectionMatrix();

    private:
        glm::vec3 getNewWorldUp();
};

template<>
SceneObjectComponentType SceneObjectComponent::componentType<CameraComponent>();

template<>
std::string SceneObjectComponent::componentTypeString<CameraComponent>();

#endif//CAMERA_COMPONENT_HPP