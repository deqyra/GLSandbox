#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "scene/scene_object.hpp"

ObjectTransform::ObjectTransform(SceneObjectPtr sceneObj) :
    Transform(glm::vec3(0.f),
              glm::quat(1.f, glm::vec3(0.f)),
              glm::vec3(1.f)),
    _transformNotifier(),
    sceneObject(sceneObj)
{

}

ObjectTransform::ObjectTransform(SceneObjectPtr sceneObj, glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
    Transform(position,
              orientation,
              scale),
    _transformNotifier(),
    sceneObject(sceneObj)
{

}

ObjectTransform& ObjectTransform::operator=(const ObjectTransform& other)
{
    _position = other._position;
    _rotation = other._rotation;
    _scale = other._scale;
    _left = other._left;
    _up = other._up;
    _forward = other._forward;
    _localVectorsOutdated = other._localVectorsOutdated;
    _modelMatrix = other._modelMatrix;
    _matrixOutdated = other._matrixOutdated;
    // The TransformNotifier and the referenced SceneObject are left untouched

    // Notify transform change
    notifyChange();
    
    return *this;
}

ObjectTransform& ObjectTransform::operator=(const Transform& other)
{
    _position = other.getPosition();
    _rotation = other.getRotation();
    _scale = other.getScale();
    _localVectorsOutdated = true;
    _matrixOutdated = true;

    // Notify transform change
    notifyChange();
    
    return *this;
}

ObjectTransform::operator Transform()
{
    return Transform(_position, _rotation, _scale);
}

/* 
 * 
 * glm::vec3 ObjectTransform::getPosition()
 * {
 *     return PlainTransform::getPosition();
 * }
 * 
 */

void ObjectTransform::setPosition(glm::vec3 position)
{
   Transform::setPosition(position);
    // Notify transform change
    notifyChange();
}

glm::vec3 ObjectTransform::translateBy(glm::vec3 translation)
{
    glm::vec3 newPosition = Transform::translateBy(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

void ObjectTransform::orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    Transform::orbit(radAngle, axis, center, selfRotate);
    // Notify transform change
    notifyChange();
}

/* 
 * 
 * glm::quat ObjectTransform::getRotation()
 * {
 *     return _orientation;
 * }
 * 
 */

void ObjectTransform::setRotation(glm::quat orientation)
{
    Transform::setRotation(orientation);
    // Notify transform change
    notifyChange();
}

glm::quat ObjectTransform::rotateBy(glm::quat rotation)
{
    glm::quat newRotation = Transform::rotateBy(rotation);
    // Notify transform change
    notifyChange();
    return newRotation;
}

glm::quat ObjectTransform::rotateBy(float radAngle, glm::vec3 axis, bool localAxis)
{
    glm::quat newRotation = Transform::rotateBy(radAngle, axis, localAxis);
    // Notify transform change
    notifyChange();
    return newRotation;
}

glm::quat ObjectTransform::lookAt(glm::vec3 target, glm::vec3 yConstraint, bool localTarget = false)
{
    glm::quat newRotation = Transform::lookAt(target, yConstraint, localTarget);
    // Notify transform change
    notifyChange();
    return newRotation;
}

/* 
 * 
 * glm::vec3 ObjectTransform::getScale()
 * {
 *     return _scale;
 * }
 * 
 */

void ObjectTransform::setScale(glm::vec3 scale)
{
    Transform::setScale(scale);
    // Notify transform change
    notifyChange();
}

glm::vec3 ObjectTransform::scaleBy(glm::vec3 scaling)
{
    glm::vec3 newScale = Transform::scaleBy(scaling),
    // Notify transform change
    notifyChange();
    return newScale;
}

void ObjectTransform::applyTo(ObjectTransform& other)
{
    other = this->Transform::applyTo((Transform) other);
}

ObjectTransform::TransformNotifier& ObjectTransform::getNotifier()
{
    return _transformNotifier;
}

void ObjectTransform::notifyChange()
{
    _transformNotifier.notify(sceneObject->id);
}
