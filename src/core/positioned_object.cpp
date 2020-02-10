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
    _modelCoordinatesUpdated(false)
{

}

PositionedObject::PositionedObject(glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
    _position(position),
    _orientation(orientation),
    _scale(scale),
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
    /*
    _orientation = glm::quat(direction);
    _orientation = glm::normalize(_orientation);
    */
    direction = glm::normalize(direction);
    glm::vec3 axis = glm::cross(direction, WORLD_Z);
    axis = glm::normalize(axis);

    float rot = glm::dot(WORLD_Z, direction);

    _orientation.x = axis.x;
    _orientation.y = axis.y;
    _orientation.z = axis.z;
    _orientation.w = rot;
    _orientation = glm::normalize(_orientation);

    _modelCoordinatesUpdated = true;
    return _orientation;
}

glm::vec3 PositionedObject::getScale()
{
    return _scale;
}

void PositionedObject::setScale(glm::vec3 scale)
{
    _scale = scale;
}

glm::vec3 PositionedObject::scale(glm::vec3 scaling)
{
    _scale[0] *= scaling[0];
    _scale[1] *= scaling[1];
    _scale[2] *= scaling[2];
    return _scale;
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
        // Scale things up
        _modelMatrix[0][0] *= _scale[0];
        _modelMatrix[1][1] *= _scale[1];
        _modelMatrix[2][2] *= _scale[2];

        _modelCoordinatesUpdated = false;
    }

    return _modelMatrix;
}
