/**
    GLTest, camera.hpp
    Purpose: Implementation of class Camera. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
*/

#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::mat4 projection, float yaw, float pitch, glm::vec3 up) :
    _worldUp(up),
    _front(glm::vec3(0.f, 0.f, -1.f)),
    _yaw(yaw),
    _pitch(pitch),
    _zoom(ZOOM),
    _projectionMatrix(projection)
{
    // Update vectors according to parameters
    updateVectors();
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

glm::vec3 Camera::getWorldUp()
{
    return _worldUp;
}

void Camera::setWorldUp(glm::vec3 up)
{
    _worldUp = glm::normalize(up);

    updateVectors();
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
    _projectionMatrix = projection;
}

glm::mat4 Camera::getViewMatrix(glm::vec3 position)
{
    return glm::lookAt(position, position + _front, _up);
}

glm::vec3 Camera::transformWorldPosition(glm::vec3 viewPos, glm::vec3 worldPosition)
{
    glm::vec4 transformedPosition = getViewMatrix(viewPos) * glm::vec4(worldPosition, 1.f);
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