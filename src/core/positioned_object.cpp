/**
    GLTest, positioned_object.cpp
    Purpose: Implementation of class PositionedObject. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include "positioned_object.hpp"

PositionedObject::PositionedObject() :
    _position(glm::vec3(0.f)),
    _orientation(glm::quat(1.f, glm::vec3(0.f))),
    _scale(glm::vec3(1.f)),
    _modelMatrix(glm::mat4(1.f)),
    _transformModifiedFlag(false),
    _matrixOutdated(false)
{

}

PositionedObject::PositionedObject(glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
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

glm::vec3 PositionedObject::getPosition()
{
    return _position;
}

void PositionedObject::setPosition(glm::vec3 position)
{
    _position = position;

    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::vec3 PositionedObject::translate(glm::vec3 translation)
{
    glm::vec4 position4 = glm::vec4(_position, 1.f);
    _position = glm::vec3(glm::translate(glm::mat4(1.f), translation) * position4);

    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _position;
}

void PositionedObject::orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    glm::vec4 tmpPos = glm::vec4(_position - center, 1.f);
    glm::mat4 orbit = glm::rotate(glm::mat4(1.f), radAngle, axis);
    tmpPos = orbit * tmpPos;
    _position = glm::vec3(tmpPos) + center;

    if (selfRotate)
    {
        rotate(-radAngle, axis);
    }

    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::quat PositionedObject::getOrientation()
{
    return _orientation;
}

void PositionedObject::setOrientation(glm::quat orientation)
{
    _orientation = orientation;
    _orientation = glm::normalize(_orientation);

    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::quat PositionedObject::rotate(glm::quat rotation)
{
    _orientation = rotation * _orientation;
    _orientation = glm::normalize(_orientation);
    _matrixOutdated = true;

    _transformModifiedFlag = true;
    return _orientation;
}

glm::quat PositionedObject::rotate(float radAngle, glm::vec3 axis, bool localAxis)
{
    if (localAxis)
    {
        axis = _orientation * axis;
    }
    _orientation = glm::rotate(_orientation, radAngle, axis);
    _orientation = glm::normalize(_orientation);

    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _orientation;
}

glm::quat PositionedObject::lookAt(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - _position);
    glm::vec3 axis = glm::cross(WORLD_Z, direction);
    axis = glm::normalize(axis);
    /*
    _orientation = glm::quat(direction);
    _orientation = glm::normalize(_orientation);
    */

    float dot = glm::dot(WORLD_Z, direction);
    float angle = glm::acos(dot);

    _orientation = glm::normalize(glm::quat(axis * angle));

    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _orientation;
}

glm::vec3 PositionedObject::getScale()
{
    return _scale;
}

void PositionedObject::setScale(glm::vec3 scale)
{
    _scale = scale;

    _transformModifiedFlag = true;
    _matrixOutdated = true;
}

glm::vec3 PositionedObject::scale(glm::vec3 scaling)
{
    _scale[0] *= scaling[0];
    _scale[1] *= scaling[1];
    _scale[2] *= scaling[2];

    _transformModifiedFlag = true;
    _matrixOutdated = true;
    return _scale;
}

glm::mat4 PositionedObject::getModelMatrix()
{
    if (_matrixOutdated)
    {
        updateMatrix();
    }

    return _modelMatrix;
}

void PositionedObject::updateMatrix()
{
    // Generate 4x4 matrix from quaternion
    _modelMatrix = glm::toMat4(glm::inverse(_orientation));
    // Scale things up
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), _scale);
    _modelMatrix = _modelMatrix * scaleMatrix;
    // Include translation to current position
    glm::vec4 position4 = glm::vec4(_position, 1.f);
    _modelMatrix[3] = position4;

    _matrixOutdated = false;
}

bool PositionedObject::transformModifiedFlagState()
{
    return _transformModifiedFlag;
}

void PositionedObject::resetTransformModifiedFlag()
{
    _transformModifiedFlag = false;
}
