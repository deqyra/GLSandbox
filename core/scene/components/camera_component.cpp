#include "camera_component.hpp"
#include "../scene_object.hpp"
#include "../scene.hpp"

#include "../../transform.hpp"

CameraComponent::CameraComponent(CameraPtr camera) :
    Component(ComponentType::Camera)
{
    setCamera(camera);
}

CameraComponent::~CameraComponent()
{

}

CameraPtr CameraComponent::getCamera()
{
    return _camera;
}

void CameraComponent::setCamera(CameraPtr camera)
{
    if (!camera)
    {
        std::string s = "NONE";
        std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
        if (sceneObject) s = std::to_string(sceneObject->id);

        s = "CameraComponent (on scene object with ID " + s + ") was passed a null camera pointer.";
        throw std::runtime_error(s.c_str());
    }

    _camera = camera;
}

glm::mat4 CameraComponent::getViewMatrix()
{
    std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
    glm::vec3 worldPosition = sceneObject->getWorldPosition();

    // Update camera and compute view matrix
    glm::vec3 newUp = getParentUp();
    _camera->setParentUp(newUp);
    return _camera->getViewMatrix(worldPosition);
}

glm::vec3 CameraComponent::worldPositionToViewSpace(glm::vec3 worldPosition)
{
    glm::vec4 viewPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(viewPosition);
}

glm::mat4 CameraComponent::getProjectionMatrix()
{
    return _camera->getProjectionMatrix();
}

glm::mat4 CameraComponent::getViewProjectionMatrix()
{
    std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
    glm::vec3 worldPosition = sceneObject->getWorldPosition();
    return _camera->getViewProjectionMatrix(worldPosition);
}

CameraComponent* CameraComponent::clone()
{
    // This shared pointer will be destroyed at the end of this scope, but responsibilty for the 
    // resources will already have been shared with the cloned CameraComponent by this point.
    CameraPtr cameraClone = std::make_shared<Camera>(*_camera);

    return new CameraComponent(cameraClone);
}

glm::vec3 CameraComponent::getParentUp()
{
    std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
    std::shared_ptr<Scene> scene = sceneObject->getScene().lock();
    glm::mat4 worldMat = scene->getWorldModelMatrix(sceneObject->id);

    // In order to get what "upwards" is (in world coordinates) for the parent scene object,
    // have (0, 1, 0) go through the world model matrix of the object.
    glm::vec4 up = glm::vec4(Transform::Y, 0.f);
    up = worldMat * up;
    return glm::vec3(up);
}

template<>
ComponentType Component::componentType<CameraComponent>()
{
    return ComponentType::Camera;
}

template<>
std::string Component::componentTypeString<CameraComponent>()
{
    return "Camera";
}
