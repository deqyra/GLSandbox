#ifndef CORE__OBJECT_TRANSFORM_HPP
#define CORE__OBJECT_TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "transform.hpp"

#include "../tools/notifier.hpp"

/* ╔════════════╗
 * ║   README   ║
 * ╚════════════╝
 * 
 * The aim of the Transform class is to represent the 3D properties of an 
 * object relative to a global frame of reference.
 * The aim of the ObjectTransform class, which inherits from Transform, is to 
 * represent the 3D properties of the SceneObject which it is attached to.
 * However, the representation is in that case relative to the frame of
 * reference of that SceneObject's parent (hereafter "the parent frame of 
 * reference").
 * 
 * For convenience, when applying a transformation to a Transform through one
 * of its methods, it is possible to specify, through a template parameter, 
 * which frame of reference the provided arguments are relative to.
 * 
 * ▫ FrameOfReference::World
 *   Indicates that the provided arguments are relative to the global frame
 *   of reference.
 * 
 * ▫ FrameOfReference::Parent
 *   - In the case of an ObjectTransform, which is attached to a SceneObject,
 *     indicates that the provided arguments are relative to the parent frame
 *     of reference.
 *   - In the case of a pure Transform, which is attached to no SceneObject and
 *     thus has no parent, the provided arguments are interpreted as being 
 *     relative to the global frame of reference, making
 *     FrameOfReference::Parent strictly equivalent to FrameOfReference::World
 *     in that context.
 * 
 * ▫ FrameOfReference::Self
 *   Indicates that the provided arguments are relative to the transform on
 *   which the transformation is about to be applied.
 * 
 * ============================================================================
 * 
 * For the ObjectTransform, all of this gives place to some convoluted logic 
 * regarding how the base Transform methods are be called in order to do what 
 * the overridden ObjectTransform methods should actually do.
 * More details in object_transform.cpp.
 */

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

// An object that has 3D-space properties : position, orientation and scale
class ObjectTransform : public Transform
{
    public:
        // A TransformNotifier callback simply needs the ID of the object whose transform was updated.
        using TransformNotifier = Notifier<const unsigned int&>;

    protected:
        // Will notify subscribers that the transform has been modified
        TransformNotifier _transformNotifier;
        // The SceneObject the transform is attached to
        SceneObjectWPtr _sceneObject;
        // The ID of the SceneObject the transform is attached to
        unsigned int _objectId;

        // Send notification to all subscribers that the transform was updated
        void notifyChange();

    public:
        ObjectTransform();
        ObjectTransform(glm::vec3 position, glm::quat orientation, glm::vec3 scale);
        ObjectTransform(const ObjectTransform& other) = delete;
        ObjectTransform& operator=(const ObjectTransform& other);
        ObjectTransform& operator=(const Transform& other);

        // Get the object the transform is attached to
        SceneObjectWPtr getSceneObject();
        // Get the object the transform is attached to
        void setSceneObject(SceneObjectPtr wSceneObj);

        // Get the position of the object - no need to overload, here for convenience
        // glm::vec3 getPosition();
        // Set the position of the object
        template<FrameOfReference Ref>
        void setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector
        template<FrameOfReference Ref>
        glm::vec3 translateBy(glm::vec3 translation);
        // Orbit the object around an axis and center
        template<FrameOfReference Ref>
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object - no need to overload, here for convenience
        // glm::quat getRotation();
        // Set the orientation of the object
        template<FrameOfReference Ref>
        void setRotation(glm::quat rotation);
        // Rotate the object by a quaternion
        glm::quat rotateBy(glm::quat rotation);
        // Rotate the object around an axis
        template<FrameOfReference Ref>
        glm::quat rotateBy(float radAngle, glm::vec3 axis);
        // Rotate the object so that its front is directed to the target position
        template<FrameOfReference Ref>
        glm::quat lookAt(glm::vec3 target, glm::vec3 yConstraint);

        // Get the scale of the object - no need to overload, here for convenience
        // glm::vec3 getScale();
        // Set the scale of the object
        template<FrameOfReference Ref>
        void setScale(glm::vec3 scale);
        // Scale the object
        glm::vec3 scaleBy(glm::vec3 scaling);

        // Apply this transform to another one
        void applyTo(ObjectTransform& other);

        // Retrieve the TransformNotifier attached to this
        TransformNotifier& getNotifier();

};

#endif//CORE__OBJECT_TRANSFORM_HPP