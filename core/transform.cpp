#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "scene/scene_object.hpp"

Transform::Transform(SceneObjectPtr sceneObj) :
    PlainTransform(glm::vec3(0.f),
                   glm::quat(1.f, glm::vec3(0.f)),
                   glm::vec3(1.f)),
    _transformNotifier(),
    sceneObject(sceneObj)
{

}

Transform::Transform(SceneObjectPtr sceneObj, glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
    PlainTransform(position,
                   orientation,
                   scale),
    _transformNotifier(),
    sceneObject(sceneObj)
{

}

Transform& Transform::operator=(const Transform& other)
{
    _position = other._position;
    _orientation = other._orientation;
    _scale = other._scale;
    _modelMatrix = other._modelMatrix;
    _matrixOutdated = other._matrixOutdated;
    // The TransformNotifier and the referenced SceneObject are left untouched

    if (_matrixOutdated)
    {
        // Generate model matrix according to parameters
        updateMatrix();
        // Notify transform change
        notifyChange();
    }
    
    return *this;
}

/* 
 * 
 * glm::vec3 Transform::getPosition()
 * {
 *     return PlainTransform::getPosition();
 * }
 * 
 */

void Transform::setPosition(glm::vec3 position)
{
   PlainTransform::setPosition(position);
    // Notify transform change
    notifyChange();
}

glm::vec3 Transform::translate(glm::vec3 translation)
{
    glm::vec3 newPosition = PlainTransform::translate(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

void Transform::orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    PlainTransform::orbit(radAngle, axis, center, selfRotate);
    // Notify transform change
    notifyChange();
}

/* 
 * 
 * glm::quat Transform::getOrientation()
 * {
 *     return _orientation;
 * }
 * 
 */

void Transform::setOrientation(glm::quat orientation)
{
    PlainTransform::setOrientation(orientation);
    // Notify transform change
    notifyChange();
}

glm::quat Transform::rotate(glm::quat rotation)
{
    glm::quat newRotation = PlainTransform::rotate(rotation);
    // Notify transform change
    notifyChange();
    return newRotation;
}

glm::quat Transform::rotate(float radAngle, glm::vec3 axis, bool localAxis)
{
    glm::quat newRotation = PlainTransform::rotate(radAngle, axis, localAxis);
    // Notify transform change
    notifyChange();
    return newRotation;
}

glm::quat Transform::lookAt(glm::vec3 target)
{
    glm::quat newRotation = PlainTransform::lookAt(target);
    // Notify transform change
    notifyChange();
    return newRotation;
}

/* 
 * 
 * glm::vec3 Transform::getScale()
 * {
 *     return _scale;
 * }
 * 
 */

void Transform::setScale(glm::vec3 scale)
{
    PlainTransform::setScale(scale);
    // Notify transform change
    notifyChange();
}

glm::vec3 Transform::scale(glm::vec3 scaling)
{
    glm::vec3 newScale = PlainTransform::scale(scaling),
    // Notify transform change
    notifyChange();
    return newScale;
}

Transform::TransformNotifier& Transform::getNotifier()
{
    return _transformNotifier;
}

void Transform::notifyChange()
{
    _transformNotifier.notify(sceneObject->id);
}
