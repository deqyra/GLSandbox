#ifndef CORE__TRANSFORM_HPP
#define CORE__TRANSFORM_HPP

#include "plain_transform.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../tools/notifier.hpp"

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

// An object that has 3D-space properties : position, orientation and scale
class Transform : public PlainTransform
{
    public:
        // A TransformNotifier callback simply needs the ID of the object whose transform was updated.
        using TransformNotifier = Notifier<const unsigned int&>;

    protected:
        // Will notify subscribers that the transform has been modified
        TransformNotifier _transformNotifier;

        // Send notification to all subscribers that the transform was updated
        void notifyChange();

    public:
        Transform(SceneObjectPtr sceneObj);
        Transform(SceneObjectPtr sceneObj, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
        Transform(const Transform& other) = delete;
        Transform& operator=(const Transform& other);

        const SceneObjectPtr sceneObject;

        // Get the position of the object - no need to overload, here for convenience
        // glm::vec3 getPosition();
        // Set the position of the object
        void setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector
        glm::vec3 translate(glm::vec3 translation);
        // Orbit the object around an axis and center
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object - no need to overload, here for convenience
        // glm::quat getOrientation();
        // Set the orientation of the object
        void setOrientation(glm::quat orientation);
        // Rotate the object by a quaternion
        glm::quat rotate(glm::quat rotation);
        // Rotate the object around an axis
        glm::quat rotate(float radAngle, glm::vec3 axis, bool localAxis = false);
        // Rotate the object so that its front is directed to the target position
        glm::quat lookAt(glm::vec3 target);

        // Get the scale of the object - no need to overload, here for convenience
        // glm::vec3 getScale();
        // Set the scale of the object
        void setScale(glm::vec3 scale);
        // Scale the object
        glm::vec3 scale(glm::vec3 scaling);

        // Retrieve the TransformNotifier attached to this
        TransformNotifier& getNotifier();
};

#endif//CORE__TRANSFORM_HPP