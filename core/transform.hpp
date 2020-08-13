#ifndef CORE__TRANSFORM_HPP
#define CORE__TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// An object that has 3D-space properties : position, orientation and scale
class Transform
{
    protected:
        // 3D position of the object
        glm::vec3 _position;
        // Quaternion representing the rotation of the object
        glm::quat _rotation;
        // 3D scale of the object
        glm::vec3 _scale;

        // Local X of the object in world coordinates
        mutable glm::vec3 _left;
        // Local Y of the object in world coordinates
        mutable glm::vec3 _up;
        // Local Z of the object in world coordinates
        mutable glm::vec3 _forward;

        // Whether the local vectors no longer reflects the transform parameters
        mutable bool _localVectorsOutdated;
        // Update the local vectors so that they reflect the transform parameters
        void updateLocalVectors() const;

        // Model matrix of the object
        mutable glm::mat4 _modelMatrix;
        // Whether the model matrix no longer reflects the transform parameters
        mutable bool _matrixOutdated;
        // Update the model matrix of the object so that it reflects the transform parameters
        void updateMatrix() const;

    public:
        static constexpr glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
        static constexpr glm::vec3 X = glm::vec3(1.f, 0.f, 0.f);
        static constexpr glm::vec3 Y = glm::vec3(0.f, 1.f, 0.f);
        static constexpr glm::vec3 Z = glm::vec3(0.f, 0.f, 1.f);

        Transform();
        Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

        // Get the position of the object
        glm::vec3 getPosition() const;
        // Set the position of the object
        void setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector
        glm::vec3 translateBy(glm::vec3 other);
        // Orbit the object around an axis and center
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object
        glm::quat getRotation() const;
        // Set the orientation of the object
        void setRotation(glm::quat orientation);
        // Rotate the object by a quaternion
        glm::quat rotateBy(glm::quat other);
        // Rotate the object around an axis
        glm::quat rotateBy(float radAngle, glm::vec3 axis, bool localAxis = false);
        // Rotate the object so that its front is directed to the target position, with respect to a constraint on the upwards direction
        // localTarget indicates whether the provided target position is local to the transform
        // yConstraint vector should always be given in world coordinates; provide the null vector to remove the constraint
        glm::quat lookAt(glm::vec3 target, glm::vec3 yConstraint, bool localTarget = false);

        // Get the scale of the object
        glm::vec3 getScale() const;
        // Set the scale of the object
        void setScale(glm::vec3 scale);
        // Scale the object by an amount
        glm::vec3 scaleBy(glm::vec3 other);

        // Get the local X vector of the object in world coordinates
        glm::vec3 left() const;
        // Get the local Y vector of the object in world coordinates
        glm::vec3 up() const;
        // Get the local Z vector of the object in world coordinates
        glm::vec3 forward() const;

        // Apply this transform to another one
        Transform applyTo(const Transform& other) const;

        // Get the model matrix of the object
        glm::mat4 getModelMatrix() const;
};

#endif//CORE__TRANSFORM_HPP