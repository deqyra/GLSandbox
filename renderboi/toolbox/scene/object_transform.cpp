#include "object_transform.hpp"

#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <renderboi/core/frame_of_reference.hpp>

#include "scene.hpp"
#include "scene_object.hpp"

using Ref = FrameOfReference;

/* ╔══════════════════════════════════════════╗
 * ║                  README                  ║
 * ║ First refer to the explicative paragraph ║
 * ║ in object_transform.hpp if you haven't.  ║
 * ╚══════════════════════════════════════════╝
 * 
 * The ObjectTransform overrides nearly every method from Transform, for two 
 * reasons.
 * 
 * ▫ The ObjectTransform comes with a Notifier, whose purpose is to emit a 
 *   notification anytime the Transform changes.
 * 
 * ▫ The 3D properties of a pure Transform are expressed relative to the global
 *   frame of reference, whereas those of an ObjectTransform are expressed 
 *   relative to the parent frame of reference.
 *   
 * Put another way, the representation is expressed relative to the parent 
 * transform in both cases, but the parent of a pure Transform turns out to be
 * the global frame of reference, whereas the parent of an ObjectTransform can
 * be any frame of reference, hence why they need to be handled differently.
 * 
 * In the following definitions of overridden members, three cases can be 
 * distinguished.
 * 
 * ▫ When arguments are provided relative to the global frame of reference,
 * the aim is mostly to express the provided arguments relative to the parent 
 * frame of reference, and then call the Parent version of the same method
 * in the base Transform instance.
 * Note: this will simply in turn call the World version of that base method,
 * but making a direct call to the World version of a method with Parent-
 * relative arguments would only add to the mess. It will be optimized out 
 * anyway.
 * 
 * ▫ When arguments are provided relative to the parent frame of reference,
 * the Parent version of the base method can be called directly with these 
 * arguments.
 * 
 * ▫ Same when arguments are provided relative to the transform's own frame of 
 * reference.
 */

ObjectTransform::ObjectTransform(const Transform transform) :
    Transform(transform),
    _transformNotifier(),
    _sceneObject(SceneObjectPtr(nullptr)),
    _objectId(0)
{

}

ObjectTransform::ObjectTransform(glm::vec3 position, glm::quat orientation, glm::vec3 scale) :
    Transform(position,
              orientation,
              scale),
    _transformNotifier(),
    _sceneObject(SceneObjectPtr(nullptr)),
    _objectId(0)
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

SceneObjectWPtr ObjectTransform::getSceneObject() const
{
    return _sceneObject;
}

void ObjectTransform::setSceneObject(const SceneObjectPtr sceneObj)
{
    _sceneObject = sceneObj;
    SceneObjectPtr sceneObject = _sceneObject.lock();

    if (sceneObject) _objectId = sceneObject->id;
    else _objectId = -1;
}

ObjectTransform::TransformNotifier& ObjectTransform::getNotifier()
{
    return _transformNotifier;
}

template<>
glm::vec3 ObjectTransform::setPosition<Ref::World>(const glm::vec3 position)
{
    // Compute the new wanted position in parent's coordinates
    
    // Get the parent object's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();
    const glm::vec3 parentScale = parentTransform.getScale();

    // Find the position coordinates relatives to the parent
    glm::vec3 positionRelativeToParent = position - parentTransform.getPosition();
    
    // The found coordinates do not take the parent rotation into account;
    // rotate them by the inverse parent rotation to fix that
    positionRelativeToParent = glm::conjugate(parentRotation) * positionRelativeToParent;
    
    // The found coordinates do not take the parent scale into account; 
    // divide them member-wise by the parent scale to fix that
    positionRelativeToParent.x /= parentScale.x;
    positionRelativeToParent.y /= parentScale.y;
    positionRelativeToParent.z /= parentScale.z;
    
    Transform::setPosition<Ref::Parent>(positionRelativeToParent);
    // Notify transform change
    notifyChange();

    return _position;
}

template<>
glm::vec3 ObjectTransform::setPosition<Ref::Parent>(const glm::vec3 position)
{
    Transform::setPosition<Ref::Parent>(position);
    // Notify transform change
    notifyChange();

    return _position;
}

template<>
glm::vec3 ObjectTransform::setPosition<Ref::Self>(const glm::vec3 position)
{
    Transform::setPosition<Ref::Self>(position);
    // Notify transform change
    notifyChange();

    return _position;
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::World>(const glm::vec3& translation)
{
    // Get the parent object's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();
    const glm::vec3 parentScale = parentTransform.getScale();

    // Rotate the translation vector by the inverse parent rotation
    // to make it relative to the parent
    glm::vec3 parentTranslation = glm::conjugate(parentRotation) * translation;
    
    // The found coordinates do not take the parent scale into account; 
    // divide them member-wise by the parent scale to fix that
    parentTranslation.x /= parentScale.x;
    parentTranslation.y /= parentScale.y;
    parentTranslation.z /= parentScale.z;

    // Translate position by found vector
    glm::vec3 newPosition = Transform::translateBy<Ref::Parent>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::Parent>(const glm::vec3& translation)
{
    const glm::vec3 newPosition = Transform::translateBy<Ref::Parent>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::Self>(const glm::vec3& translation)
{
    const glm::vec3 newPosition = Transform::translateBy<Ref::Self>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
glm::vec3 ObjectTransform::orbit<Ref::World>(
    const float radAngle,
    const glm::vec3& axis,
    const glm::vec3& center,
    const bool selfRotate
)
{
    // Get the parent's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();
    const glm::vec3 parentScale = parentTransform.getScale();

    // Rotate the orbital axis by the inverse parent rotation
    // to make it relative to the parent
    const glm::vec3 parentAxis = glm::conjugate(parentRotation) * axis;

    // Find the center of the orbital movement relative to the parent, in world coordinates
    glm::vec3 parentCenter = center - parentTransform.getPosition();

    // Rotate the center coordinates by the inverse parent rotation
    // to make them relative to the parent
    parentCenter = glm::conjugate(parentRotation) * parentCenter;
    
    // The found coordinates do not take the parent scale into account; 
    // divide them member-wise by the parent scale to fix that
    parentCenter.x /= parentScale.x;
    parentCenter.y /= parentScale.y;
    parentCenter.z /= parentScale.z;

    Transform::orbit<Ref::Parent>(radAngle, parentAxis, parentCenter, selfRotate);

    // Notify transform change
    notifyChange();

    return _position;
}

template<>
glm::vec3 ObjectTransform::orbit<Ref::Parent>(
    const float radAngle,
    const glm::vec3& axis,
    const glm::vec3& center,
    const bool selfRotate
)
{
    Transform::orbit<Ref::Parent>(radAngle, axis, center, selfRotate);
    // Notify transform change
    notifyChange();

    return _position;
}


template<>
glm::vec3 ObjectTransform::orbit<Ref::Self>(
    const float radAngle,
    const glm::vec3& axis,
    const glm::vec3& center,
    const bool selfRotate
)
{
    Transform::orbit<Ref::Self>(radAngle, axis, center, selfRotate);
    // Notify transform change
    notifyChange();

    return _position;
}

template<>
glm::quat ObjectTransform::setRotation<Ref::World>(const glm::quat rotation)
{
    // Get the parent's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();

    // Combine rotation with inverse parent rotation to make it relative to the parent
    const glm::quat rotationRelativeToParent = glm::conjugate(parentRotation) * rotation;
    Transform::setRotation<Ref::Parent>(rotationRelativeToParent);
    // Notify transform change
    notifyChange();

    return _rotation;
}

template<>
glm::quat ObjectTransform::setRotation<Ref::Parent>(const glm::quat rotation)
{
    Transform::setRotation<Ref::Parent>(rotation);
    // Notify transform change
    notifyChange();

    return _rotation;
}

template<>
glm::quat ObjectTransform::setRotation<Ref::Self>(const glm::quat rotation)
{
    Transform::setRotation<Ref::Self>(rotation);
    // Notify transform change
    notifyChange();

    return _rotation;
}

glm::quat ObjectTransform::rotateBy(const glm::quat& rotation)
{
    const glm::quat newRotation = Transform::rotateBy(rotation);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::rotateBy<Ref::World>(const float radAngle, const glm::vec3& axis)
{
    // Get the parent's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();

    // Rotate axis by inverse parent rotation to make it relative to the parent
    const glm::vec3 parentAxis = glm::conjugate(parentRotation) * axis;

    const glm::quat newRotation = Transform::rotateBy<Ref::Parent>(radAngle, parentAxis);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::rotateBy<Ref::Parent>(const float radAngle, const glm::vec3& axis)
{
    const glm::quat newRotation = Transform::rotateBy<Ref::Parent>(radAngle, axis);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::rotateBy<Ref::Self>(const float radAngle, const glm::vec3& axis)
{
    const glm::quat newRotation = Transform::rotateBy<Ref::Self>(radAngle, axis);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::lookAt<Ref::World>(const glm::vec3& target, const glm::vec3& yConstraint)
{
    // Get the parent's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::quat parentRotation = parentTransform.getRotation();
    const glm::vec3 parentScale = parentTransform.getScale();

    // Find the target position relative to the parent, in world coordinates
    glm::vec3 parentTarget = target - parentTransform.getPosition();

    // Rotate the target coordinates by the inverse parent rotation
    // to make them relative to the parent
    parentTarget = glm::conjugate(parentRotation) * parentTarget;
    
    // The found coordinates do not take the parent scale into account; 
    // divide them member-wise by the parent scale to fix that
    parentTarget.x /= parentScale.x;
    parentTarget.y /= parentScale.y;
    parentTarget.z /= parentScale.z;

    glm::quat newRotation = Transform::lookAt<Ref::Parent>(target, yConstraint);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::lookAt<Ref::Parent>(const glm::vec3& target, const glm::vec3& yConstraint)
{
    const glm::quat newRotation = Transform::lookAt<Ref::Parent>(target, yConstraint);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::quat ObjectTransform::lookAt<Ref::Self>(const glm::vec3& target, const glm::vec3& yConstraint)
{
    const glm::quat newRotation = Transform::lookAt<Ref::Self>(target, yConstraint);
    // Notify transform change
    notifyChange();
    return newRotation;
}

template<>
glm::vec3 ObjectTransform::setScale<Ref::World>(const glm::vec3 scale)
{
    // Get the parent's world transform
    const SceneObjectPtr thisObject = _sceneObject.lock();
    const ScenePtr scene = thisObject->getScene();
    const unsigned int parentId = scene->getParentId(_objectId);
    const Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    const glm::vec3 parentScale = parentTransform.getScale();

    const glm::vec3 newScale = {
        scale.x / parentScale.x,
        scale.y / parentScale.y,
        scale.z / parentScale.z
    };

    Transform::setScale<Ref::Parent>(newScale);
    // Notify transform change
    notifyChange();

    return _scale;
}

template<>
glm::vec3 ObjectTransform::setScale<Ref::Parent>(const glm::vec3 scale)
{
    Transform::setScale<Ref::Parent>(scale);
    // Notify transform change
    notifyChange();

    return _scale;
}

template<>
glm::vec3 ObjectTransform::setScale<Ref::Self>(const glm::vec3 scale)
{
    Transform::setScale<Ref::Self>(scale);
    // Notify transform change
    notifyChange();

    return _scale;
}

glm::vec3 ObjectTransform::scaleBy(const glm::vec3& scaling)
{
    Transform::scaleBy(scaling);
    // Notify transform change
    notifyChange();
    
    return _scale;
}

Transform ObjectTransform::applyOver(const ObjectTransform& other) const
{
    return this->Transform::applyOver((Transform) other);
}

Transform ObjectTransform::compoundFrom(const ObjectTransform& other) const
{
    return this->Transform::compoundFrom((Transform) other);
}

void ObjectTransform::notifyChange() const
{
    _transformNotifier.notify(_objectId);
}
