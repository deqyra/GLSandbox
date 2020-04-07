#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../camera.hpp"

#include <string>

class CameraComponent : public Component
{
    public:
        CameraComponent(CameraPtr camera);
        virtual ~CameraComponent();

        CameraPtr camera;

        virtual glm::mat4 getViewMatrix();
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition);
        virtual glm::mat4 getProjectionMatrix();
        virtual glm::mat4 getViewProjectionMatrix();

        virtual CameraComponent* clone();

    private:
        glm::vec3 getNewWorldUp();
};

template<>
ComponentType Component::componentType<CameraComponent>();

template<>
std::string Component::componentTypeString<CameraComponent>();

#endif//CAMERA_COMPONENT_HPP