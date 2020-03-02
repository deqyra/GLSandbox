/**
    GLTest, camera.hpp
    Purpose: Implementation of class Camera. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
*/

#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::mat4 projection, float yaw, float pitch, glm::vec3 up) :
    _position(position),
    _worldUp(up),
    _front(glm::vec3(0.f, 0.f, -1.f)),
    _yaw(yaw),
    _pitch(pitch),
    _zoom(ZOOM),
    _vectorsUpdated(false),
    _viewUpdated(false),
    _projectionUpdated(true),
    _viewMatrix(glm::mat4(1.f)),
    _projectionMatrix(projection),
    _viewProjectionMatrix(projection)
{
    // Update vectors according to parameters
    updateVectors();
    // Generate view matrix
    getViewMatrix();
    // Generate the view projection matrix
    getViewProjectionMatrix();
}

Camera::~Camera()
{

}

void Camera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));

    _vectorsUpdated = true;
}

void Camera::processMovement(Direction dir, float velocity)
{
    switch (dir)
    {
        case Direction::FORWARD:
            _position += _front * velocity;
            break;
        case Direction::BACKWARD:
            _position -= _front * velocity;
            break;
        case Direction::LEFT:
            _position -= _right * velocity;
            break;
        case Direction::RIGHT:
            _position += _right * velocity;
            break;
    }

    updateVectors();
}

void Camera::processRotation(float yawOffset, float pitchOffset, bool constrainPitch)
{
    _yaw += yawOffset;
    _pitch += pitchOffset;

    if (constrainPitch)
    {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    updateVectors();
}

void Camera::processZoom(float scrollOffset)
{
    if (_zoom >= 1.0f && _zoom <= 45.0f)
        _zoom -= scrollOffset;
    if (_zoom <= 1.0f)
        _zoom = 1.0f;
    if (_zoom >= 45.0f)
        _zoom = 45.0f;
}

glm::vec3 Camera::getPosition()
{
    return _position;
}

void Camera::setPosition(glm::vec3 position)
{
    _position = position;
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
    _projectionMatrix = projection;
    _projectionUpdated = true;
}

glm::mat4 Camera::getViewMatrix()
{
    if (_vectorsUpdated)
    {
        _viewMatrix = glm::lookAt(_position, _position + _front, _up);
        _vectorsUpdated = false;
        _viewUpdated = true;
    }

    return _viewMatrix;
}

glm::vec3 Camera::transformWorldPosition(glm::vec3 worldPosition)
{
    glm::vec4 viewPosition = _viewMatrix * glm::vec4(worldPosition, 1.f);
    return glm::vec3(viewPosition);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return _projectionMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    if (_viewUpdated || _projectionUpdated)
    {
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
        _viewUpdated = false;
        _projectionUpdated = false;
    }

    return _viewProjectionMatrix;
}