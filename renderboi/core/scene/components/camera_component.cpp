#include "camera_component.hpp"
#include "../scene_object.hpp"
#include "../scene.hpp"

#include "../../object_transform.hpp"

CameraComponent::CameraComponent(SceneObjectPtr sceneObject, CameraPtr camera) :
    Component(ComponentType::Camera, sceneObject)
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
        throw std::runtime_error("CameraComponent: cannot set camera from a null pointer.");
    }

    _camera = camera;
}

glm::mat4 CameraComponent::getViewMatrix()
{
    std::shared_ptr<SceneObject> sceneObject = _sceneObject;
    Transform worldTransform = sceneObject->getWorldTransform();

    // Update camera and compute view matrix
    _camera->setParentTransform(worldTransform);
    return _camera->getViewMatrix();
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
    glm::vec3 worldPosition = _sceneObject->getWorldTransform().getPosition();
    return _camera->getViewProjectionMatrix(worldPosition);
}

CameraComponent* CameraComponent::clone(SceneObjectPtr newParent)
{
    // This shared pointer will be destroyed at the end of this scope, but responsibilty for the 
    // resources will already have been shared with the cloned CameraComponent by this point.
    CameraPtr cameraClone = std::make_shared<Camera>(*_camera);

    return new CameraComponent(newParent, cameraClone);
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

template<>
bool Component::multipleInstancesAllowed<CameraComponent>()
{
    return false;
}
