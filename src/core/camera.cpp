/**
    GLTest, camera.hpp
    Purpose: Implementation of class Camera. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
*/

#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, float yaw, float pitch, glm::vec3 up) :
    _pos(position),
    _worldUp(up),
    _front(glm::vec3(0.f, 0.f, -1.f)),
    _yaw(yaw),
    _pitch(pitch),
    _zoom(ZOOM),
    _vectorsUpdated(false),
    _viewMatrix(glm::mat4(1.f))
{
    // Update vectors according to parameters
    updateVectors();
    // Generate view matrix
    getViewMatrix();
}

glm::mat4 Camera::getViewMatrix()
{
    if (_vectorsUpdated)
    {
        _viewMatrix = glm::lookAt(_pos, _pos + _front, _up);
        _vectorsUpdated = false;
    }

    return _viewMatrix;
}

glm::vec3 Camera::getPosition()
{
    return _pos;
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
            _pos += _front * velocity;
            break;
        case Direction::BACKWARD:
            _pos -= _front * velocity;
            break;
        case Direction::LEFT:
            _pos -= _right * velocity;
            break;
        case Direction::RIGHT:
            _pos += _right * velocity;
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