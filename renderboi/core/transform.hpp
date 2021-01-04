#ifndef RENDERBOI__CORE__TRANSFORM_HPP
#define RENDERBOI__CORE__TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "frame_of_reference.hpp"
#include "interfaces/basis_provider.hpp"

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

namespace Renderboi
{

/// @brief Manages the 3D-space properties of an object: position, orientation
/// and scale. Refer to the README section at the top of the .hpp file for more
/// info.
class Transform : public BasisProvider
{
    protected:
        /// @brief 3D position of the object.
        glm::vec3 _position;

        /// @brief Quaternion representing the rotation of the object.
        glm::quat _rotation;

        /// @brief 3D scale of the object.
        glm::vec3 _scale;

        /// @brief Local X axis of the object in world coordinates.
        mutable glm::vec3 _left;

        /// @brief Local Y axis of the object in world coordinates.
        mutable glm::vec3 _up;

        /// @brief Local Z axis of the object in world coordinates.
        mutable glm::vec3 _forward;

        /// @brief Whether the local vectors no longer reflects the transform
        /// parameters.
        mutable bool _localVectorsOutdated;

        /// @brief Update the local vectors so that they reflect the transform
        /// parameters.
        void _updateLocalVectors() const;

        /// @brief Model matrix of the object.
        mutable glm::mat4 _modelMatrix;

        /// @brief Whether the model matrix no longer reflects the transform 
        ///parameters.
        mutable bool _matrixOutdated;

        /// @brief Update the model matrix of the object so that it reflects the
        /// transform parameters.
        void _updateMatrix() const;

    public:
        /// @brief Coordinates of the null vector.
        static constexpr glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
        
        /// @brief Coordinates of the global X vector.
        static constexpr glm::vec3 X = glm::vec3(1.f, 0.f, 0.f);
        
        /// @brief Coordinates of the global Y vector.
        static constexpr glm::vec3 Y = glm::vec3(0.f, 1.f, 0.f);
        
        /// @brief Coordinates of the global Z vector.
        static constexpr glm::vec3 Z = glm::vec3(0.f, 0.f, 1.f);

        Transform();
   
        /// @param position Base position of the transform.
        /// @param rotation Base orientation of the transform.
        /// @param scale Base scale of the transform.
        Transform(const glm::vec3 position, const glm::quat rotation, const glm::vec3 scale);

        /// @brief Get the position of the object.
        ///
        /// @return The position of the object.
        glm::vec3 getPosition() const;

        /// @brief Set the position of the object.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param position The new position the object should have.
        ///
        /// @return The new position of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 setPosition(const glm::vec3 position);

        /// @brief Translate the position of the object by a 3D vector.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param translation Vector by which the object should be translated.
        ///
        /// @return The new position of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 translateBy(const glm::vec3& other);

        /// @brief Orbit the object around an axis and center.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param radAngle Angle by which the object should orbit.
        /// @param axis Axis about which the object should orbit.
        ///
        /// @return The new position of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 orbit(const float radAngle, const glm::vec3& axis, const glm::vec3& center, bool selfRotate = false);

        /// @brief Get the rotation of the object.
        ///
        /// @return The rotation of the object.
        glm::quat getRotation() const;

        /// @brief Set the rotation of the object.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param rotation The new rotation the object should have.
        ///
        /// @return The new rotation of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::quat setRotation(glm::quat rotation);

        /// @brief Rotate the object by a quaternion.
        ///
        /// @param rotation The quaternion by which to rotate the object.
        ///
        /// @return The new rotation of the object relative to its parent.
        glm::quat rotateBy(const glm::quat& other);

        /// @brief Rotate the object around an axis.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param radAngle Angle by which the object should rotate.
        /// @param axis Axis about which the object should rotate.
        ///
        /// @return The new rotation of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::quat rotateBy(const float radAngle, const glm::vec3& axis);

        /// @brief Rotate the object so that its front (local Z axis) is 
        /// directed towards the target position.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param target Position the object should be looking towards.
        /// @param yConstraint Coplanarity constraint between the new local Z
        /// and Y axes of the object after rotation. Provide the null vector
        /// for no constraint.
        ///
        /// @return The new rotation of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::quat lookAt(const glm::vec3& target, glm::vec3 yConstraint);

        /// @brief Get the scale of the object.
        ///
        /// @return The scale of the object.
        glm::vec3 getScale() const;

        /// @brief Set the scale of the object.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param scale The new scale the object should have.
        ///
        /// @return The new scale of the object relative to its parent.
        template<FrameOfReference Ref>
        void setScale(glm::vec3 scale);

        /// @brief Scale the object by an amount on all three axes.
        ///
        /// @param scale The amount the object should be scaled by on the X,
        /// Y and Z axes.
        ///
        /// @return The new scale of the object relative to its parent.
        glm::vec3 scaleBy(const glm::vec3& other);

        /// @brief Apply this transform on top of another one.
        ///
        /// @param other The transform on top of which this one should be 
        /// applied.
        ///
        /// @return The resulting transform.
        Transform applyOver(const Transform& other) const;

        /// @brief Compute and return the transform differential transform
        /// between [*this] and [other]. If X is such a Transform, then 
        /// [X.applyOver(other) == *this] is true.
        ///
        /// @param other The transform from which the differential transform
        /// needs to be computed.
        ///
        /// @return The resulting transform.
        Transform compoundFrom(const Transform& other) const;

        /// @brief Get the matrix which applies the parameters of the transform
        /// to any point which it multiplies.
        ///
        /// @return The model matrix corresponding to the transform.
        glm::mat4 getModelMatrix() const;

        /////////////////////////////////////////////
        ///                                       ///
        /// Methods overridden from BasisProvider ///
        ///                                       ///
        /////////////////////////////////////////////

        /// @brief Get the local X vector of the object in world coordinates.
        ///
        /// @return The local X vector of the object, in world coordinates.
        glm::vec3 left() const override;

        /// @brief Get the local Y vector of the object in world coordinates.
        ///
        /// @return The local Y vector of the object, in world coordinates.
        glm::vec3 up() const override;

        /// @brief Get the local Z vector of the object in world coordinates.
        ///
        /// @return The local Z vector of the object, in world coordinates.
        glm::vec3 forward() const override;
};

}//namespace Renderboi

#endif//RENDERBOI__CORE__TRANSFORM_HPP