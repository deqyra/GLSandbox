#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transform.hpp"

Camera::Camera(const Camera& other) :
    _front(other._front),
    _right(other._right),
    _up(other._up),
    _parentUp(other._parentUp),
    _parentUpRotation(other._parentUpRotation),
    _zoom(other._zoom),
    _yaw(other._yaw),
    _pitch(other._pitch),
    _projectionMatrix(other._projectionMatrix)
{

}

Camera::Camera(glm::mat4 projection, float yaw, float pitch, float zoom, glm::vec3 up) :
    _front(glm::vec3(0.f, 0.f, -1.f)),
    _right(glm::vec3(1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _parentUp(glm::vec3(0.f, 1.f, 0.f)),
    _parentUpRotation(),
    _zoom(zoom),
    _yaw(yaw),
    _pitch(pitch),
    _projectionMatrix(projection)
{
    // Update vectors according to parameters
    setParentUp(up);
    
    // updateVectors();     // updateVectors is already called within setParentUp
}

Camera::~Camera()
{

}

void Camera::updateVectors()
{
    glm::vec3 front;
    // Compute new front based on yaw and pitch
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    // Apply the rotation between the parent's current upwards direction and world Y
    front = _parentUpRotation * front;
    _front = glm::normalize(front);

    // Compute new right and up from new front and parent up
    _right = glm::normalize(glm::cross(_front, _parentUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::processRotation(float yawOffset, float pitchOffset)
{
    _yaw += yawOffset;
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

glm::vec3 Camera::front()
{
    return _front;
}

glm::vec3 Camera::right()
{
    return _right;
}

glm::vec3 Camera::up()
{
    return _up;
}

glm::vec3 Camera::getParentUp()
{
    return _parentUp;
}

void Camera::setParentUp(glm::vec3 up)
{
    _parentUp = glm::normalize(up);
    _parentUpRotation = glm::quat(Transform::Y, _parentUp);

    updateVectors();
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
    _projectionMatrix = projection;
}

glm::mat4 Camera::getViewMatrix(glm::vec3 viewPoint)
{
    return glm::lookAt(viewPoint, viewPoint + _front, _up);
}

glm::vec3 Camera::worldPositionToViewSpace(glm::vec3 viewPoint, glm::vec3 worldPosition)
{
    glm::vec4 transformedPosition = getViewMatrix(viewPoint) * glm::vec4(worldPosition, 1.f);
    return glm::vec3(transformedPosition);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return _projectionMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix(glm::vec3 viewPos)
{
    return _projectionMatrix * getViewMatrix(viewPos);
}