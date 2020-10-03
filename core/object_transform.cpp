#include "object_transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "frame_of_reference.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"

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
 * relative arguments would only add to the mess (plus it will be optimized 
 * out anyway).
 * 
 * ▫ When arguments are provided relative to the parent frame of reference,
 * the Parent version of the base method can be called directly with these 
 * arguments.
 * 
 * ▫ Same when arguments are provided relative to the transform's own frame of 
 * reference.
 */

ObjectTransform::ObjectTransform() :
    Transform(glm::vec3(0.f),
              glm::quat(1.f, glm::vec3(0.f)),
              glm::vec3(1.f)),
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

SceneObjectWPtr ObjectTransform::getSceneObject()
{
    return _sceneObject;
}

void ObjectTransform::setSceneObject(SceneObjectPtr wSceneObj)
{
    _sceneObject = wSceneObj;
    SceneObjectPtr sceneObject = _sceneObject.lock();

    if (sceneObject) _objectId = sceneObject->id;
    else _objectId = -1;
}

/* 
 * 
 * glm::vec3 ObjectTransform::getPosition()
 * {
 *     return Transform::getPosition();
 * }
 * 
 */

template<>
void ObjectTransform::setPosition<Ref::World>(glm::vec3 position)
{
    // Compute the new wanted position in parent's coordinates
    
    // Get the parent object's world transform
    SceneObjectPtr thisObject = _sceneObject.lock();
    ScenePtr scene = thisObject->getScene().lock();
    unsigned int parentId = scene->getParentId(_objectId);
    Transform parentTransform = scene->getWorldTransform(parentId);

    // Retrieve some stuff
    glm::quat parentRotation = parentTransform.getRotation();
    glm::vec3 parentScale = parentTransform.getScale();

    // Find the position coordinates relatives to the parent
    glm::vec3 positionRelativeToParent = position - parentTransform.getPosition();
    
    // The found coordinates do not take the parent rotation into account;
    // rotate them by the inverse parent rotation to fix it
    positionRelativeToParent = glm::conjugate(parentRotation) * positionRelativeToParent * parentRotation;
    
    // The found coordinates do not take the parent scale into account; 
    // divide them member-wise by the parent scale to fix it
    positionRelativeToParent.x /= parentScale.x;
    positionRelativeToParent.y /= parentScale.y;
    positionRelativeToParent.z /= parentScale.z;
    
    translateBy<Ref::Parent>(positionRelativeToParent);
}

template<>
void ObjectTransform::setPosition<Ref::Parent>(glm::vec3 position)
{
    Transform::setPosition<Ref::Parent>(position);
    // Notify transform change
    notifyChange();
}

template<>
void ObjectTransform::setPosition<Ref::Self>(glm::vec3 position)
{
    Transform::setPosition<Ref::Self>(position);
    // Notify transform change
    notifyChange();
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::World>(glm::vec3 translation)
{
    // Get the parent object's world transform
    SceneObjectPtr thisObject = _sceneObject.lock();
    ScenePtr scene = thisObject->getScene().lock();
    unsigned int parentId = scene->getParentId(_objectId);
    Transform parentTransform = scene->getWorldTransform(parentId);
    // ...as well as the object's world transform
    Transform worldTransform = scene->getWorldTransform(_objectId);
    
    glm::quat parentRotation = parentTransform.getRotation();
    // Rotating the world translation vector by the inverse parent rotation gives what the translation vector should 
    // be in the parent frame of reference in order to apply the same translation in the global frame of reference
    glm::vec3 parentTranslation = glm::conjugate(parentRotation) * translation * parentRotation;

    // Translate position by found vector:
    // For an ObjectTransform, translating by some vector in the parent's frame of reference is 
    // equivalent to translating by that vector in the world's frame of refence for a pure Transform
    glm::vec3 newPosition = Transform::translateBy<Ref::World>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::Parent>(glm::vec3 translation)
{
    // For an ObjectTransform, translating by some vector in the parent's frame of reference is 
    // equivalent to translating by that vector in the world's frame of refence for a pure Transform
    glm::vec3 newPosition = Transform::translateBy<Ref::World>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
glm::vec3 ObjectTransform::translateBy<Ref::Self>(glm::vec3 translation)
{
    glm::vec3 newPosition = Transform::translateBy<Ref::Self>(translation);
    // Notify transform change
    notifyChange();
    return newPosition;
}

template<>
void ObjectTransform::orbit<Ref::World>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    // Get the object's world transform
    SceneObjectPtr thisObject = _sceneObject.lock();
    ScenePtr scene = thisObject->getScene().lock();
    Transform worldTransform = scene->getWorldTransform(_objectId);
    // ...as well as the parent's
    unsigned int parentId = scene->getParentId(_objectId);
    Transform parentTransform = scene->getWorldTransform(parentId);

    glm::quat parentRotation = parentTransform.getRotation();
    // Rotating the world axis vector by the inverse parent rotation gives what the axis vector should 
    // be in the parent frame of reference in order for a rotation around it to be the same as in the global frame of reference
    glm::vec3 parentAxis = glm::conjugate(parentRotation) * axis * parentRotation;

    // Find the center of the orbital movement relative to the parent
    glm::vec3 parentCenter = center - parentTransform.getPosition();

    // Apply the orbital movement in the parent's frame of reference,
    // which is equivalent to the global frame of reference for a pure Transform
    Transform::orbit<Ref::World>(radAngle, parentAxis, parentCenter, selfRotate);

    // Notify transform change
    notifyChange();
}

template<>
void ObjectTransform::orbit<Ref::Parent>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    Transform::orbit<Ref::World>(radAngle, axis, center, selfRotate);
    // Notify transform change
    notifyChange();
}


template<>
void ObjectTransform::orbit<Ref::Self>(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate)
{
    // Get the object's world transform
    SceneObjectPtr thisObject = _sceneObject.lock();
    ScenePtr scene = thisObject->getScene().lock();
    Transform worldTransform = scene->getWorldTransform(_objectId);

    // Recompute provided parameters according to parent transform
    glm::vec3 worldCenter = center.x * worldTransform.left()
                          + center.y * worldTransform.up()
                          + center.z * worldTransform.forward();
    worldCenter += worldTransform.getPosition();

    // Inverse rotation applied to the axis gives the axis in world coordinates
    glm::quat rotation = worldTransform.getRotation();
    glm::vec3 worldAxis = glm::conjugate(rotation) * axis * rotation;

    orbit<Ref::World>(radAngle, worldAxis, worldCenter, selfRotate);
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

glm::quat ObjectTransform::lookAt(glm::vec3 target, glm::vec3 yConstraint, bool localTarget)
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
    glm::vec3 newScale = Transform::scaleBy(scaling);
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
    _transformNotifier.notify(_objectId);
}
