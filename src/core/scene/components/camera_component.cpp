#include "../scene_object.hpp"
#include "camera_component.hpp"

using CompType = SceneObjectComponentType;

CameraComponent::CameraComponent(std::shared_ptr<ViewProjectionProvider> camera) :
    SceneObjectComponent(CompType::Camera),
    camera(camera)
{

}

CameraComponent::~CameraComponent()
{

}

glm::mat4 CameraComponent::getViewMatrix()
{
    auto strongSceneObj = sceneObject.lock();
    return camera->getViewMatrix() * strongSceneObj->getModelMatrix();
}

glm::vec3 CameraComponent::transformWorldPosition(glm::vec3 worldPosition)
{
    return camera->transformWorldPosition(worldPosition);
}

glm::mat4 CameraComponent::getProjectionMatrix()
{
    return camera->getProjectionMatrix();
}

glm::mat4 CameraComponent::getViewProjectionMatrix()
{
    return camera->getViewProjectionMatrix();
}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<CameraComponent>()
{
    return SceneObjectComponentType::Camera;
}

template<>
std::string SceneObjectComponent::componentTypeString<CameraComponent>()
{
    return "Camera";
}
