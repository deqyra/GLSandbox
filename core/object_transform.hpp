#ifndef CORE__OBJECT_TRANSFORM_HPP
#define CORE__OBJECT_TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "transform.hpp"

#include "../tools/notifier.hpp"

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

// An object that has 3D-space properties : position, orientation and scale
class ObjectTransform : public Transform
{
    friend class SceneObject;

    public:
        // A TransformNotifier callback simply needs the ID of the object whose transform was updated.
        using TransformNotifier = Notifier<const unsigned int&>;

    protected:
        // Pointer to the SceneObject this transform is attached to
        SceneObjectWPtr _sceneObject;

        // Will notify subscribers that the transform has been modified
        TransformNotifier _transformNotifier;

        // Send notification to all subscribers that the transform was updated
        void notifyChange();

    public:
        ObjectTransform(SceneObjectWPtr sceneObj);
        ObjectTransform(SceneObjectWPtr sceneObj, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
        ObjectTransform(const ObjectTransform& other) = delete;
        ObjectTransform& operator=(const ObjectTransform& other);
        ObjectTransform& operator=(const Transform& other);

        SceneObjectWPtr getSceneObject();

        // Get the position of the object - no need to overload, here for convenience
        // glm::vec3 getPosition();
        // Set the position of the object
        void setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector
        glm::vec3 translateBy(glm::vec3 translation);
        // Orbit the object around an axis and center
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object - no need to overload, here for convenience
        // glm::quat getRotation();
        // Set the orientation of the object
        void setRotation(glm::quat orientation);
        // Rotate the object by a quaternion
        glm::quat rotateBy(glm::quat rotation);
        // Rotate the object around an axis
        glm::quat rotateBy(float radAngle, glm::vec3 axis, bool localAxis = false);
        // Rotate the object so that its front is directed to the target position
        // localTarget indicates whether the provided target position is local to the transform
        // yConstraint vector should always be given in world coordinates; provide the null vector to remove the constraint
        glm::quat lookAt(glm::vec3 target, glm::vec3 yConstraint, bool localTarget = false);

        // Get the scale of the object - no need to overload, here for convenience
        // glm::vec3 getScale();
        // Set the scale of the object
        void setScale(glm::vec3 scale);
        // Scale the object
        glm::vec3 scaleBy(glm::vec3 scaling);

        // Apply this transform to another one
        void applyTo(ObjectTransform& other);

        // Retrieve the TransformNotifier attached to this
        TransformNotifier& getNotifier();
};

#endif//CORE__OBJECT_TRANSFORM_HPP