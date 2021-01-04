#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transform.hpp"

namespace Renderboi
{

Camera::Camera(const Camera& other) :
    _forward(other._forward),
    _left(other._left),
    _up(other._up),
    _parentTransform(other._parentTransform),
    _yaw(other._yaw),
    _pitch(other._pitch),
    _zoomFactor(other._zoomFactor),
    _verticalFov(other._verticalFov),
    _aspectRatio(other._aspectRatio),
    _nearDistance(other._nearDistance),
    _farDistance(other._farDistance),
    _projectionMatrix(other._projectionMatrix),
    _projectionMatrixOutdated(other._projectionMatrixOutdated)
{

}

Camera::Camera(
    const CameraParameters cameraParameters,
    const ProjectionParameters projectionParameters,
    const Transform parentTransform
) :
    _forward(glm::vec3(0.f, 0.f, -1.f)),
    _left(glm::vec3(-1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _parentTransform(parentTransform),
    _yaw(cameraParameters.yaw + 90.f),
    _pitch(cameraParameters.pitch),
    _zoomFactor(cameraParameters.zoomFactor),
    _verticalFov(projectionParameters.verticalFov),
    _aspectRatio(projectionParameters.aspectRatio),
    _nearDistance(projectionParameters.nearDistance),
    _farDistance(projectionParameters.farDistance),
    _projectionMatrixOutdated(true)
{
    // Update vectors according to parameters and parent transform
    setParentTransform(parentTransform);
    _updateProjectionMatrix();
}

void Camera::processRotation(const float yawOffset, const float pitchOffset)
{
    _yaw += yawOffset;
    if (_yaw > 180.f)  _yaw -= 360.f;
    if (_yaw < -180.f) _yaw += 360.f;

    _pitch += pitchOffset;
    // Constrain pitch
    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    _updateVectors();
}

void Camera::processZoom(const float scrollOffset)
{
    // Not used
    if (_zoomFactor >= 1.0f && _zoomFactor <= 45.0f)
        _zoomFactor -= scrollOffset;
    if (_zoomFactor <= 1.0f)
        _zoomFactor = 1.0f;
    if (_zoomFactor >= 45.0f)
        _zoomFactor = 45.0f;
}

Transform Camera::getParentTransform() const
{
    return _parentTransform;
}

void Camera::setParentTransform(const Transform& parentTransform)
{
    _parentTransform = parentTransform;

    _updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(_parentTransform.getPosition(), _parentTransform.getPosition() + _forward, _up);
}

glm::vec3 Camera::worldPositionToViewSpace(const glm::vec3& worldPosition)
{
    glm::vec4 transformedPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(transformedPosition);
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(const float aspectRatio)
{
    _aspectRatio = aspectRatio;
    _projectionMatrixOutdated = true;
}

float Camera::getVerticalFov() const
{
    return _verticalFov;
}

void Camera::setVerticalFov(const float verticalFov)
{
    _verticalFov = verticalFov;
    _projectionMatrixOutdated = true;
}

float Camera::getNearDistance() const
{
    return _nearDistance;
}

void Camera::setNearDistance(const float nearDistance)
{
    _nearDistance = nearDistance;
    _projectionMatrixOutdated = true;
}

float Camera::getFarDistance() const
{
    return _farDistance;
}

void Camera::setFarDistance(const float farDistance)
{
    _farDistance = farDistance;
    _projectionMatrixOutdated = true;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    if (_projectionMatrixOutdated) _updateProjectionMatrix();

    return _projectionMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return getProjectionMatrix() * getViewMatrix();
}

void Camera::_updateVectors() const
{
    glm::vec3 forward;
    // Compute new front based on yaw and pitch
    forward.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    forward.y = glm::sin(glm::radians(_pitch));
    forward.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    // Apply the world rotation of the parent
    forward = _parentTransform.getRotation() * forward;
    _forward = glm::normalize(forward);

    // Compute new left and up from new forward and parent up
    _left = glm::normalize(glm::cross(_parentTransform.up(), _forward));
    _up = glm::normalize(glm::cross(_forward, _left));
}

void Camera::_updateProjectionMatrix() const
{
    _projectionMatrix = glm::perspective(_verticalFov, _aspectRatio, _nearDistance, _farDistance);
    _projectionMatrixOutdated = false;
}

glm::vec3 Camera::forward() const
{
    return _forward;
}

glm::vec3 Camera::left() const
{
    return _left;
}

glm::vec3 Camera::up() const
{
    return _up;
}

}//namespace Renderboi
