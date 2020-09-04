#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform() :
    _position(glm::vec3(0.f)),
    _rotation(glm::quat(1.f, glm::vec3(0.f))),
    _scale(glm::vec3(1.f)),
    _left(glm::vec3(1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _forward(glm::vec3(0.f, 0.f, 1.f)),
    _localVectorsOutdated(false),
    _modelMatrix(glm::mat4(1.f)),
    _matrixOutdated(false)
{

}

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) :
    _position(position),
    _rotation(glm::normalize(rotation)),
    _scale(scale),
    _left(glm::vec3(1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _forward(glm::vec3(0.f, 0.f, 1.f)),
    _localVectorsOutdated(true),
    _modelMatrix(glm::mat4(1.f)),
    _matrixOutdated(true)
{

}

glm::vec3 Transform::getPosition() const
{
    return _position;
}

void Transform::setPosition(glm::vec3 position)
{
    _position = position;

    // Update flags
    _matrixOutdated = true;
}

glm::vec3 Transform::translateBy(glm::vec3 other)
{
    // Translate the current position
    glm::vec4 position4 = glm::vec4(_position, 1.f);
    glm::vec4 translated = glm::translate(glm::mat4(1.f), other) * position4;
    _position = glm::vec3(translated);

    // Update flags
    _matrixOutdated = true;
    return _position;
}

void Transform::orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    // Orbit around the axis and center
    glm::vec4 tmpPos = glm::vec4(_position - center, 1.f);
    glm::mat4 orbit = glm::rotate(glm::mat4(1.f), radAngle, axis);
    tmpPos = orbit * tmpPos;
    _position = glm::vec3(tmpPos) + center;

    // Update rotation if needed
    if (selfRotate)
    {
        rotateBy(radAngle, axis);
    }

    // Update flags
    _matrixOutdated = true;
}

glm::quat Transform::getRotation() const
{
    return _rotation;
}

void Transform::setRotation(glm::quat orientation)
{
    _rotation = glm::normalize(orientation);

    // Update flags
    _matrixOutdated = true;
    _localVectorsOutdated = true;
}

glm::quat Transform::rotateBy(glm::quat other)
{
    // Rotate the object
    _rotation = glm::normalize(other * _rotation);

    // Update flags
    _matrixOutdated = true;
    _localVectorsOutdated = true;
    return _rotation;
}

glm::quat Transform::rotateBy(float radAngle, glm::vec3 axis, bool localAxis)
{
    if (!localAxis)
    {
        axis = _rotation * axis * glm::conjugate(_rotation);
    }

    // Rotate the object
    float cos = glm::cos(radAngle * 0.5f);
    float sin = glm::sin(radAngle * 0.5f);

    glm::vec3 tmp = glm::normalize(axis);
    glm::quat newRotation = glm::quat(cos, sin*axis.x, sin*axis.y, sin*axis.z);

    rotateBy(newRotation);

    return _rotation;
}

glm::quat Transform::lookAt(glm::vec3 target, glm::vec3 yConstraint, bool localTarget)
{
    // Local vectors are going to be needed so update them
    if (_localVectorsOutdated) updateLocalVectors();

    // Find new direction to look towards
    glm::vec3 direction = glm::normalize(target - _position);
    // If the target coordinates are given local to the position, the target is also the direction to look towards
    if (localTarget) direction = glm::normalize(target);

    // Find axis of rotation 
    glm::vec3 axis = glm::normalize(glm::cross(_forward, direction));

    // Find the angle by which to rotate around the found axis
    float dot = glm::dot(_forward, direction);
    float angle = glm::acos(dot);

    rotateBy(angle, axis);
    updateLocalVectors();

    // Check whether the Y constraint is not null and solvable
    if ((yConstraint != glm::vec3(0.f)) && (glm::normalize(yConstraint) != -_forward))
    {
        // The aim is to rotate around the new forward vector
        // so that the new up vector is coplanar with both the 
        // new forward vector and the Y constraint.
        // This means the cross products between those must be equal.

        // Find the desired left direction 
        glm::vec3 desiredLeft = glm::normalize(glm::cross(yConstraint, _forward));

        // Find the angle between the actual left and the desired left directions
        dot = glm::dot(_left, desiredLeft);
        angle = glm::acos(dot);

        // The rotation axis is either _forward or -_forward,
        // this is a safe way to find out which
        axis = glm::normalize(glm::cross(_left, desiredLeft));

        // Rotate around the found axis 
        rotateBy(angle, axis);
        _localVectorsOutdated = true;
    }

    // Update flags
    _matrixOutdated = true;
    return _rotation;
}

glm::vec3 Transform::getScale() const
{
    return _scale;
}

void Transform::setScale(glm::vec3 scale)
{
    _scale = scale;

    // Update flags
    _matrixOutdated = true;
}

glm::vec3 Transform::scaleBy(glm::vec3 scaling)
{
    // Scale the object
    _scale[0] *= scaling[0];
    _scale[1] *= scaling[1];
    _scale[2] *= scaling[2];

    // Update flags
    _matrixOutdated = true;
    return _scale;
}

glm::vec3 Transform::left() const
{
    if (_localVectorsOutdated)
    {
        updateLocalVectors();
    }

    return _left;
}

glm::vec3 Transform::up() const
{
    if (_localVectorsOutdated)
    {
        updateLocalVectors();
    }

    return _up;
}

glm::vec3 Transform::forward() const
{
    if (_localVectorsOutdated)
    {
        updateLocalVectors();
    }

    return _forward;
}


glm::mat4 Transform::getModelMatrix() const
{
    if (_matrixOutdated)
    {
        updateMatrix();
    }

    return _modelMatrix;
}

Transform Transform::applyTo(const Transform& other) const
{
    // Compute new transform position:
    // other position scaled and rotated + this position
    glm::vec3 newPosition = glm::vec3(
        other._position[0] * _scale[0],
        other._position[1] * _scale[1],
        other._position[2] * _scale[2]
    );
    newPosition = _rotation * newPosition * glm::conjugate(_rotation);
    newPosition += _position;

    // Combine rotations
    glm::quat newRotation = glm::normalize(_rotation * other._rotation);

    // Multiply scales
    glm::vec3 newScale = glm::vec3(
        _scale[0] * other._scale[0],
        _scale[1] * other._scale[1],
        _scale[2] * other._scale[2]
    );

    return Transform(newPosition, newRotation, newScale);
}

void Transform::updateLocalVectors() const
{
    // Transform world basis vectors according to rotation
    _forward = glm::normalize(_rotation * Z * glm::conjugate(_rotation));
    _up      = glm::normalize(_rotation * Y * glm::conjugate(_rotation));
    _left    = glm::normalize(_rotation * X * glm::conjugate(_rotation));

    _localVectorsOutdated = false;
}

void Transform::updateMatrix() const
{
    // Generate 4x4 matrix from quaternion
    _modelMatrix = glm::toMat4(glm::inverse(_rotation));

    // Scale things up
    _modelMatrix[0][0] *= _scale[0];
    _modelMatrix[1][1] *= _scale[1];
    _modelMatrix[2][2] *= _scale[2];

    // Include translation to current position
    _modelMatrix[3] = glm::vec4(_position, 1.f);

    _matrixOutdated = false;
}
