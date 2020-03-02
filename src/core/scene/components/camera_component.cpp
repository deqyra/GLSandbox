#include "../scene_object.hpp"
#include "camera_component.hpp"
#include "../../scene.hpp"

using CompType = SceneObjectComponentType;

CameraComponent::CameraComponent(std::shared_ptr<Camera> camera) :
    SceneObjectComponent(CompType::Camera),
    camera(camera)
{

}

CameraComponent::~CameraComponent()
{

}

glm::mat4 CameraComponent::getViewMatrix()
{
    std::shared_ptr<SceneObject> strongSceneObj = sceneObject.lock();
    glm::vec3 worldPosition = strongSceneObj->getWorldPosition();

    glm::vec3 newUp = getNewWorldUp();
    camera->setWorldUp(newUp);
    return camera->getViewMatrix(worldPosition);
}

glm::vec3 CameraComponent::transformWorldPosition(glm::vec3 worldPosition)
{
    glm::vec4 viewPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(viewPosition);
}

glm::mat4 CameraComponent::getProjectionMatrix()
{
    return camera->getProjectionMatrix();
}

glm::mat4 CameraComponent::getViewProjectionMatrix()
{
    return camera->getViewProjectionMatrix();
}

glm::vec3 CameraComponent::getNewWorldUp()
{
    std::shared_ptr<SceneObject> obj = sceneObject.lock();
    std::shared_ptr<Scene> scene = obj->scene.lock();
    glm::mat4 worldMat = scene->getWorldModelMatrix(obj->id);
    glm::vec4 up = glm::vec4(WORLD_Y, 0.f);
    up = worldMat * up;
    return glm::vec3(up);
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
