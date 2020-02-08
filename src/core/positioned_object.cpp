/**
    GLTest, positioned_object.cpp
    Purpose: Implementation of class PositionedObject. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 08/02/2020
 */
#include "positioned_object.hpp"

/*
PositionedObject::PositionedObject() :
    _position(DEFAULT_POSITION),
    _orientation(DEFAULT_ORIENTATION),
    _modelMatrix(glm::mat4(1.f)),
    _modelCoordinatesUpdated(false)
{

}
*/
PositionedObject::PositionedObject(glm::vec3 position, glm::vec3 orientation) :
    _position(position),
    _orientation(orientation),
    _modelMatrix(glm::mat4(1.f)),
    _modelCoordinatesUpdated(true)
{
    // Generate model matrix according to parameters
    getModelMatrix();
}

glm::vec3 PositionedObject::getPosition()
{
    return _position;
}

void PositionedObject::setPosition(glm::vec3 position)
{
    _position = position;
    _modelCoordinatesUpdated = true;
}

glm::vec3 PositionedObject::translate(glm::vec3 translation)
{
    glm::vec4 position4 = glm::vec4(_position, 1.f);
    _position = glm::vec3(glm::translate(glm::mat4(1.f), translation) * position4);

    _modelCoordinatesUpdated = true;
    return _position;
}

glm::quat PositionedObject::getOrientation()
{
    return _orientation;
}

void PositionedObject::setOrientation(glm::quat orientation)
{
    _orientation = orientation;
    _orientation = glm::normalize(_orientation);
    _modelCoordinatesUpdated = true;
}

glm::quat PositionedObject::rotate(glm::quat rotation)
{
    _orientation = rotation * _orientation;
    _orientation = glm::normalize(_orientation);
    return _orientation;
}

glm::quat PositionedObject::rotate(float radAngle, glm::vec3 axis)
{
    _orientation = glm::rotate(_orientation, radAngle, axis);
    _orientation = glm::normalize(_orientation);

    _modelCoordinatesUpdated = true;
    return _orientation;
}

glm::quat PositionedObject::lookAt(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - _position);
    _orientation = glm::quat(direction, WORLD_Z);
    _orientation = glm::normalize(_orientation);

    _modelCoordinatesUpdated = true;
    return _orientation;
}

glm::mat4 PositionedObject::getModelMatrix()
{
    if (_modelCoordinatesUpdated)
    {        
        // Generate 4x4 matrix from quaternion
        _modelMatrix = glm::toMat4(glm::inverse(_orientation));
        // Include translation to current position
        glm::vec4 position4 = glm::vec4(_position, 1.f);
        _modelMatrix[3] = position4;

        _modelCoordinatesUpdated = false;
    }

    return _modelMatrix;
}
