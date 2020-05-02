#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform() :
    _position(glm::vec3(0.f)),
    _orientation(glm::quat(1.f, glm::vec3(0.f))),
    _scale(glm::vec3(1.f)),
    _modelMatrix(glm::mat4(1.f)),
    _transformModifiedFlag(false),
    _matrixOutdated(false)
{

}

Transform::Transform(glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
    _position(position),
    _orientation(orientation),
    _scale(scale),
    _modelMatrix(glm::mat4(1.f)),
    _transformModifiedFlag(false),
    _matrixOutdated(true)
{
    // Generate model matrix according to parameters
    updateMatrix();
}

Transform::Transform(const Transform& other) :
    _position(other._position),
    _orientation(other._orientation),
    _scale(other._scale),
    _modelMatrix(other._modelMatrix),
    _transformModifiedFlag(other._transformModifiedFlag),
    _matrixOutdated(other._matrixOutdated)
{
    if (_matrixOutdated)
    {
        // Generate model matrix according to parameters
        updateMatrix();
    }
}

Transform& Transform::operator=(const Transform& other)
{
    _position = other._position;
    _orientation = other._orientation;
    _scale = other._scale;
    _modelMatrix = other._modelMatrix;
    _transformModifiedFlag = other._transformModifiedFlag;
    _matrixOutdated = other._matrixOutdated;
    if (_matrixOutdated)
    {
        // Generate model matrix according to parameters
        updateMatrix();
    }
    
    return *this;
}


glm::vec3 Transform::getPosition()
{
    return _position;
}

void Transform::setPosition(glm::vec3 position)
{
    _position = position;

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::vec3 Transform::translate(glm::vec3 translation)
{
    // Translate the current position
    glm::vec4 position4 = glm::vec4(_position, 1.f);
    glm::vec4 translated = glm::translate(glm::mat4(1.f), translation) * position4;
    _position = glm::vec3(translated);

    // Update flags
    _transformModifiedFlag = true;
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
        rotate(-radAngle, axis);
    }

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::quat Transform::getOrientation()
{
    return _orientation;
}

void Transform::setOrientation(glm::quat orientation)
{
    _orientation = orientation;
    _orientation = glm::normalize(_orientation);

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::quat Transform::rotate(glm::quat rotation)
{
    // Rotate the object
    _orientation = rotation * _orientation;
    _orientation = glm::normalize(_orientation);

    // Update flags
    _matrixOutdated = true;
    _transformModifiedFlag = true;
    return _orientation;
}

glm::quat Transform::rotate(float radAngle, glm::vec3 axis, bool localAxis)
{
    if (!localAxis)
    {
        axis = glm::inverse(_orientation) * axis;
    }

    // Rotate the object
    _orientation = glm::rotate(_orientation, radAngle, axis);
    _orientation = glm::normalize(_orientation);

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _orientation;
}

glm::quat Transform::lookAt(glm::vec3 target)
{
    // FIXME

    glm::vec3 direction = glm::normalize(target - _position);
    glm::vec3 axis = glm::cross(Z, direction);
    axis = glm::normalize(axis);
    /*
    _orientation = glm::quat(direction);
    _orientation = glm::normalize(_orientation);
    */

    float dot = glm::dot(Z, direction);
    float angle = glm::acos(dot);

    _orientation = glm::normalize(glm::quat(axis * angle));

    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _orientation;
}

glm::vec3 Transform::getScale()
{
    return _scale;
}

void Transform::setScale(glm::vec3 scale)
{
    _scale = scale;

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::vec3 Transform::scale(glm::vec3 scaling)
{
    // Scale the object
    _scale[0] *= scaling[0];
    _scale[1] *= scaling[1];
    _scale[2] *= scaling[2];

    // Update flags
    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _scale;
}

glm::mat4 Transform::getModelMatrix()
{
    if (_matrixOutdated)
    {
        updateMatrix();
    }

    return _modelMatrix;
}

void Transform::updateMatrix()
{
    // Generate 4x4 matrix from quaternion
    _modelMatrix = glm::toMat4(glm::inverse(_orientation));
    // Scale things up
    _modelMatrix[0][0] *= _scale[0];
    _modelMatrix[1][1] *= _scale[1];
    _modelMatrix[2][2] *= _scale[2];
    // Include translation to current position
    _modelMatrix[3] = glm::vec4(_position, 1.f);

    _matrixOutdated = false;
}

bool Transform::transformModifiedFlagState()
{
    return _transformModifiedFlag;
}

void Transform::resetTransformModifiedFlag()
{
    _transformModifiedFlag = false;
}
