#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transform.hpp"

Camera::Camera(const Camera& other) :
    _forward(other._forward),
    _left(other._left),
    _up(other._up),
    _parentTransform(other._parentTransform),
    _zoom(other._zoom),
    _yaw(other._yaw),
    _pitch(other._pitch),
    _projectionMatrix(other._projectionMatrix)
{

}

Camera::Camera(glm::mat4 projection, float yaw, float pitch, float zoom, Transform parentTransform) :
    _forward(glm::vec3(0.f, 0.f, -1.f)),
    _left(glm::vec3(1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _parentTransform(parentTransform),
    _zoom(zoom),
    _yaw(yaw + 90.f),
    _pitch(pitch),
    _projectionMatrix(projection)
{
    // Update vectors according to parameters
    setParentTransform(parentTransform);
}

void Camera::updateVectors()
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

void Camera::processRotation(float yawOffset, float pitchOffset)
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

    updateVectors();
}

void Camera::processZoom(float scrollOffset)
{
    // Not used
    if (_zoom >= 1.0f && _zoom <= 45.0f)
        _zoom -= scrollOffset;
    if (_zoom <= 1.0f)
        _zoom = 1.0f;
    if (_zoom >= 45.0f)
        _zoom = 45.0f;
}

glm::vec3 Camera::forward()
{
    return _forward;
}

glm::vec3 Camera::left()
{
    return _left;
}

glm::vec3 Camera::up()
{
    return _up;
}

Transform Camera::getParentTransform()
{
    return _parentTransform;
}

void Camera::setParentTransform(Transform parentTransform)
{
    _parentTransform = parentTransform;

    updateVectors();
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
    _projectionMatrix = projection;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_parentTransform.getPosition(), _parentTransform.getPosition() + _forward, _up);
}

glm::vec3 Camera::worldPositionToViewSpace(glm::vec3 worldPosition)
{
    glm::vec4 transformedPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(transformedPosition);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return _projectionMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix(glm::vec3 viewPoint)
{
    return _projectionMatrix * getViewMatrix();
}