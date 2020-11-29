#ifndef RENDERBOI__TOOLBOX__OBJECT_TRANSFORM_HPP
#define RENDERBOI__TOOLBOX__OBJECT_TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <renderboi/core/transform.hpp>

#include <cpptools/notifier.hpp>

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
 * as to how the base methods are called, in order to do what the overridden 
 * methods should actually do.
 * More details in object_transform.cpp.
 */

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

/// @brief Wraps a Transform and is attached to a SceneObject. Refer to the
/// README section at the top of the .hpp file for more info.
class ObjectTransform : public Transform
{
    public:
        using TransformNotifier = Notifier<const unsigned int&>;

    protected:
        /// @brief Will notify subscribers that the transform has been modified.
        TransformNotifier _transformNotifier;
        /// @brief The SceneObject the transform is attached to.
        SceneObjectWPtr _sceneObject;
        /// @brief The ID of the SceneObject the transform is attached to.
        unsigned int _objectId;

        /// @brief Notify all subscribers that the transform was updated.
        void notifyChange();

    public:
        /// @param transform Base state of the object transform.
        ObjectTransform(Transform transform);

        /// @param position Base position of the transform.
        /// @param rotation Base orientation of the transform.
        /// @param scale Base scale of the transform.
        ObjectTransform(glm::vec3 position = glm::vec3(0.f), glm::quat orientation = glm::quat(1.f, glm::vec3(0.f)), glm::vec3 scale = glm::vec3(1.f));

        ObjectTransform(const ObjectTransform& other) = delete;

        ObjectTransform& operator=(const ObjectTransform& other);

        ObjectTransform& operator=(const Transform& other);

        /// @brief Get the object the transform is attached to.
        ///
        /// @return Pointer to the scene object the transform is attached to.
        SceneObjectWPtr getSceneObject();

        /// @brief Set which object the transform is attached to
        ///
        /// @param sceneObj A pointer to the scene object the transform should
        /// be attached to.
        void setSceneObject(SceneObjectPtr sceneObj);

        /// @brief Set the position of the object.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param position The new position the object should have.
        ///
        /// @return The new position of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 setPosition(glm::vec3 position);

        /// @brief Translate the position of the object by a 3D vector.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param translation Vector by which the object should be translated.
        ///
        /// @return The new position of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 translateBy(glm::vec3 translation);

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
        glm::vec3 orbit(float radAngle, glm::vec3 axis, glm::vec3 center, bool selfRotate = false);

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
        glm::quat rotateBy(glm::quat rotation);

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
        glm::quat rotateBy(float radAngle, glm::vec3 axis);

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
        glm::quat lookAt(glm::vec3 target, glm::vec3 yConstraint);

        /// @brief Set the scale of the object.
        ///
        /// @tparam Ref Literal describing the frame of reference in which the
        /// arguments are provided to the function.
        ///
        /// @param scale The new scale the object should have.
        ///
        /// @return The new scale of the object relative to its parent.
        template<FrameOfReference Ref>
        glm::vec3 setScale(glm::vec3 scale);

        /// @brief Scale the object by an amount on all three axes.
        ///
        /// @param scale The amount the object should be scaled by on the X,
        /// Y and Z axes.
        ///
        /// @return The new scale of the object relative to its parent.
        glm::vec3 scaleBy(glm::vec3 scaling);

        /// @brief Apply this transform on top of another one.
        ///
        /// @param other The transform on top of which this one should be 
        /// applied.
        ///
        /// @return The resulting transform.
        Transform applyOver(const ObjectTransform& other) const;

        /// @brief Compute and return the transform differential transform
        /// between [*this] and [other]. If X is such a Transform, then 
        /// [X.applyOver(other) == *this] is true.
        ///
        /// @param other The transform from which the differential transform
        /// needs to be computed.
        ///
        /// @return The resulting transform.
        Transform compoundFrom(const ObjectTransform& other) const;

        /// @brief Retrieve the notifier attached to this transform.
        ///
        /// @brief The notifier attached to this transform.
        TransformNotifier& getNotifier();

};

#endif//RENDERBOI__TOOLBOX__OBJECT_TRANSFORM_HPP