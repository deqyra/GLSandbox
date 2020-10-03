#ifndef CORE__TRANSFORM_HPP
#define CORE__TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "frame_of_reference.hpp"

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
 */

// An object which holds 3D-space properties : position, orientation and scale
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
        // Set the position of the object and return the new position in world coordinates
        template<FrameOfReference Ref>
        glm::vec3 setPosition(glm::vec3 position);
        // Translate the position of the object by a 3D vector and return the new position in world coordinates
        template<FrameOfReference Ref>
        glm::vec3 translateBy(glm::vec3 other);
        // Orbit the object around an axis and center
        template<FrameOfReference Ref>
        void orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

        // Get the orientation of the object
        glm::quat getRotation() const;
        // Set the orientation of the object and return the new orientation in world coordinates
        template<FrameOfReference Ref>
        glm::quat setRotation(glm::quat rotation);
        // Rotate the object by a quaternion and return the new orientation in world coordinates
        glm::quat rotateBy(glm::quat other);
        // Rotate the object around an axis and return the new orientation in world coordinates
        template<FrameOfReference Ref>
        glm::quat rotateBy(float radAngle, glm::vec3 axis);
        // Rotate the object so that its own Z axis (front) is directed to the target position, with respect to a constraint on the new YZ plane, and return the new orientation in world coordinates
        template<FrameOfReference Ref>
        glm::quat lookAt(glm::vec3 target, glm::vec3 yConstraint);

        // Get the scale of the object
        glm::vec3 getScale() const;
        // Set the scale of the object
        template<FrameOfReference Ref>
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