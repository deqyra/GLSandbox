#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/quaternion.hpp>

#include "frame_of_reference.hpp"

using Ref = FrameOfReference;

/* ╔════════════════════════════════════╗
 * ║               README               ║
 * ║ Refer to the explicative paragraph ║
 * ║  in transform.hpp if you haven't.  ║
 * ╚════════════════════════════════════╝
 */

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

template<>
glm::vec3 Transform::setPosition<Ref::World>(glm::vec3 position)
{
    _position = position;

    // Update flags
    _matrixOutdated = true;
    return position;
}

template<>
glm::vec3 Transform::setPosition<Ref::Parent>(glm::vec3 position)
{
    return setPosition<Ref::World>(position);
}

template<>
glm::vec3 Transform::setPosition<Ref::Self>(glm::vec3 position)
{
    return translateBy<Ref::Self>(position);
}

template<>
glm::vec3 Transform::translateBy<Ref::World>(glm::vec3 other)
{
    // Translate the current position
    _position += other;

    // Update flags
    _matrixOutdated = true;
    return _position;
}

template<>
glm::vec3 Transform::translateBy<Ref::Parent>(glm::vec3 other)
{
    return translateBy<Ref::World>(other);
}

template<>
glm::vec3 Transform::translateBy<Ref::Self>(glm::vec3 other)
{
    if (_localVectorsOutdated) updateLocalVectors();

    // Recalculate provided translation relative to the world
    glm::vec3 worldTranslation = other.x * _scale.x * _left   
                               + other.y * _scale.y * _up     
                               + other.z * _scale.z * _forward;
    return translateBy<Ref::World>(worldTranslation);
}

template<>
void Transform::orbit<Ref::World>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    // Orbit around the axis and center
    glm::vec4 tmpPos = glm::vec4(_position - center, 1.f);
    glm::mat4 orbit = glm::rotate(glm::mat4(1.f), radAngle, axis);
    tmpPos = orbit * tmpPos;
    _position = glm::vec3(tmpPos) + center;

    // Update rotation if needed
    if (selfRotate)
    {
        rotateBy<Ref::World>(radAngle, axis);
    }

    // Update flags
    _matrixOutdated = true;
}

template<>
void Transform::orbit<Ref::Parent>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    orbit<Ref::World>(radAngle, axis, center, selfRotate);
}

template<>
void Transform::orbit<Ref::Self>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    if (_localVectorsOutdated) updateLocalVectors();

    // Recalculate provided parameters relative to the world
    glm::vec3 worldCenter = center.x * _scale.x * _left
                          + center.y * _scale.y * _up
                          + center.z * _scale.z * _forward;
    worldCenter += _position;

    // Inverse rotation applied to the axis gives the axis in world coordinates
    glm::vec3 worldAxis = glm::conjugate(_rotation) * axis * _rotation;
    orbit<Ref::World>(radAngle, worldAxis, worldCenter, selfRotate);
}

glm::quat Transform::getRotation() const
{
    return _rotation;
}

template<>
glm::quat Transform::setRotation<Ref::World>(glm::quat rotation)
{
    _rotation = glm::normalize(rotation);

    // Update flags
    _matrixOutdated = true;
    _localVectorsOutdated = true;

    return rotation;
}

template<>
glm::quat Transform::setRotation<Ref::Parent>(glm::quat rotation)
{
    return setRotation<Ref::World>(rotation);
}

template<>
glm::quat Transform::setRotation<Ref::Self>(glm::quat rotation)
{
    return rotateBy(rotation);
}

// This function does not need to be templated with a frame of reference for its arguments:
// A rotation in one frame of reference will be encoded by the exact same quaternion in any other frame of reference.
// Thus, rotating by a quaternion is independant of any and all frames of references.
glm::quat Transform::rotateBy(glm::quat other)
{
    // Rotate the object
    _rotation = glm::normalize(other * _rotation);

    // Update flags
    _matrixOutdated = true;
    _localVectorsOutdated = true;
    return _rotation;
}

template<>
glm::quat Transform::rotateBy<Ref::World>(float radAngle, glm::vec3 axis)
{
    // Compute quaternion associated with wanted rotation
    float cos = glm::cos(radAngle * 0.5f);
    float sin = glm::sin(radAngle * 0.5f);

    glm::vec3 tmp = glm::normalize(axis);
    glm::quat newRotation = glm::quat(cos, sin*tmp.x, sin*tmp.y, sin*tmp.z);

    // Rotate the object
    rotateBy(newRotation);

    return _rotation;
}

template<>
glm::quat Transform::rotateBy<Ref::Parent>(float radAngle, glm::vec3 axis)
{
    rotateBy<Ref::World>(radAngle, axis);
}

template<>
glm::quat Transform::rotateBy<Ref::Self>(float radAngle, glm::vec3 axis)
{
    // Recalculate provided axis relative to the world
    // Inverse rotation applied to the axis gives the axis in world coordinates
    glm::vec3 worldAxis = glm::conjugate(_rotation) * axis * _rotation;
    rotateBy<Ref::World>(radAngle, worldAxis);
}

template<>
glm::quat Transform::lookAt<Ref::World>(glm::vec3 target, glm::vec3 yConstraint)
{
    // Local vectors are going to be needed so update them
    if (_localVectorsOutdated) updateLocalVectors();

    // Find new direction to look towards
    glm::vec3 direction = glm::normalize(target - _position);

    // Find axis of rotation 
    glm::vec3 axis = glm::normalize(glm::cross(_forward, direction));


    // Find the angle by which to rotate around the found axis
    float dot = glm::dot(_forward, direction);
    // Works because both vectors are unit, so the only significant term in the
    // trig expression of their dot product is the cos of the angle they make
    float angle = glm::acos(dot);

    rotateBy<Ref::World>(angle, axis);
    updateLocalVectors();

    // Check whether the Y constraint is not null and solvable
    if ((yConstraint != glm::vec3(0.f)) && (glm::normalize(yConstraint) != -_forward))
    {
        // The aim is to rotate around the new forward vector so that the up vector becomes 
        // coplanar with both the new forward vector and the provided Y constraint.
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
        rotateBy<Ref::World>(angle, axis);
        _localVectorsOutdated = true;
    }

    // Update flags
    _matrixOutdated = true;
    return _rotation;
}

template<>
glm::quat Transform::lookAt<Ref::Parent>(glm::vec3 target, glm::vec3 yConstraint)
{
    lookAt<Ref::World>(target, yConstraint);
}

template<>
glm::quat Transform::lookAt<Ref::Self>(glm::vec3 target, glm::vec3 yConstraint)
{
    // Recalculate provided parameters relative to the world
    glm::vec3 worldTarget = target.x * left()
                          + target.y * up()
                          + target.z * forward();
    worldTarget += _position;

    // Inverse rotation applied to the axis gives the axis in world coordinates
    glm::vec3 worldYConstraint = glm::conjugate(_rotation) * yConstraint * _rotation;
    lookAt<Ref::World>(worldTarget, worldYConstraint);
}

glm::vec3 Transform::getScale() const
{
    return _scale;
}

template<>
void Transform::setScale<Ref::World>(glm::vec3 scale)
{
    _scale = scale;

    // Update flags
    _matrixOutdated = true;
}

template<>
void Transform::setScale<Ref::Parent>(glm::vec3 scale)
{
    setScale<Ref::World>(scale);
}

template<>
void Transform::setScale<Ref::Self>(glm::vec3 scale)
{
    scaleBy(scale);
}

glm::vec3 Transform::scaleBy(glm::vec3 other)
{
    // Scale the object
    _scale.x *= other.x;
    _scale.y *= other.y;
    _scale.z *= other.z;

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
    // _rotation is a unit quat, simply conjugating it actually inverts it 
    glm::quat inv = glm::conjugate(_rotation);

    // Transform world basis vectors according to rotation
    _forward = glm::normalize(_rotation * Z * inv);
    _up      = glm::normalize(_rotation * Y * inv);
    _left    = glm::normalize(_rotation * X * inv);

    _localVectorsOutdated = false;
}

void Transform::updateMatrix() const
{
    // Generate 4x4 matrix from quaternion
    //_modelMatrix = glm::toMat4(glm::inverse(_rotation));
    _modelMatrix = glm::toMat4(_rotation);

    // Scale things up
    _modelMatrix[0][0] *= _scale[0];
    _modelMatrix[1][1] *= _scale[1];
    _modelMatrix[2][2] *= _scale[2];

    // Include translation to current position
    _modelMatrix[3] = glm::vec4(_position, 1.f);

    _matrixOutdated = false;
}
