#ifndef RENDERBOI__CORE__CAMERA_HPP
#define RENDERBOI__CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "interfaces/basis_provider.hpp"
#include "transform.hpp"

namespace Renderboi
{

/// @brief A camera managing its own orientation and providing view and projection matrices.
class Camera : public BasisProvider
{
private:
    /// @brief Direction the camera is facing.
    mutable glm::vec3 _forward;

    /// @brief Direction to the left of the camera.
    mutable glm::vec3 _left;

    /// @brief Upward direction of the camera.
    mutable glm::vec3 _up;

    /// @brief Upwards direction of the object the camera is attached to.
    Transform _parentTransform;

    /// @brief Yaw (rotation around Y) of the camera in degrees.
    float _yaw;

    /// @brief Pitch (rotation around X) of the camera in degrees.
    float _pitch;

    /// @brief Zoom factor of the camera.
    float _zoomFactor;

    /// @brief Angle (in radians) of the vertical field of view of the 
    /// camera.
    float _verticalFov;

    /// @brief Aspect ratio of the image produced by the camera.
    float _aspectRatio;

    /// @brief Distance to the near plane of the camera, before which 
    /// elements will not be rendered.
    float _nearDistance;

    /// @brief Distance to the far plane of the camera, beyond which 
    /// elements will not be rendered.
    float _farDistance;

    /// @brief Projection matrix of the camera. Provides perspective and 
    /// lens effect.
    mutable glm::mat4 _projectionMatrix;

    /// @brief Whether or not the projection matrix needs recalculating in 
    /// order to reflect new parameters.
    mutable bool _projectionMatrixOutdated;

    /// @brief Recalculate vectors based on yaw, pitch and parent rotation.
    void _updateVectors() const;

    /// @brief Recalculate the projection matrix based on the registered 
    /// parameters.
    void _updateProjectionMatrix() const;

public:
    struct CameraParameters
    {
        float yaw;
        float pitch;
        float zoomFactor;
    };

    static constexpr CameraParameters DefaultCameraParameters = {0.f, 0.f, 1.f};

    struct ProjectionParameters
    {
        float verticalFov;
        float aspectRatio;
        float nearDistance;
        float farDistance;
    };

    static constexpr ProjectionParameters DefaultProjectionParameters = {glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f};

    /// @param cameraParameters Parameters of the camera: yaw, pitch, zoom.
    /// @param projectionParameters Parameters of the projection used by the
    /// camera: vertical FOV, aspect ratio, near and far plane distances.
    /// @param parentTransform Transform of the object the camera is 
    /// attached to.
    Camera(
        const CameraParameters cameraParameters = DefaultCameraParameters,
        const ProjectionParameters projectionParameters = DefaultProjectionParameters,
        const Transform parentTransform = Transform()
    );

    Camera(const Camera& other);

    /// @brief Update the rotation of the camera to fit recorded offsets in 
    /// yaw and pitch.
    ///
    /// @param yawOffset How much the camera was rotated around Y in degrees.
    /// @param pitchOffset How much the camera was rotated around X in degrees.
    void processRotation(const float yawOffset, const float pitchOffset);
    
    /// @brief DO NOT USE - Update the projection matrix of the camera to 
    /// zoom in or out.
    ///
    /// @param scrollOffset How much the camera was zoomed in or out.
    void processZoom(const float scrollOffset);

    /// @brief Get the world transform of the parent of the camera.
    ///
    /// @return The world transform of the parent of the camera.
    Transform getParentTransform() const;

    /// @brief Update the registered world transform of the parent of the camera.
    ///
    /// @param transform New world transform of the parent of the camera.
    void setParentTransform(const Transform& parentTransform);

    /// @brief Get the view matrix of the camera from its view point.
    ///
    /// @return The view matrix of the camera.
    glm::mat4 getViewMatrix() const;
    
    /// @brief Transform vector coordinates from world space to view space 
    /// from the camera view point.
    ///
    /// @param worldPosition Coordinates to transform into view space.
    ///
    /// @return The transformed coordinates.
    glm::vec3 worldPositionToViewSpace(const glm::vec3& worldPosition);

    /// @brief Get the vertical field of view of the camera.
    ///
    /// @return The angle of the vertical field of view of the camera.
    float getVerticalFov() const;

    /// @brief Set the vertical field of view of the camera.
    ///
    /// @param verticalFov The new angle of the vertical field of view of 
    /// the camera.
    void setVerticalFov(const float verticalFov);

    /// @brief Get the aspect ratio of the image produced by the camera.
    ///
    /// @return The aspect ratio of the image produced by the camera.
    float getAspectRatio() const;

    /// @brief Set the aspect ratio of the image produced by the camera.
    ///
    /// @param aspectRatio The new aspect ratio of the camera.
    void setAspectRatio(const float aspectRatio);

    /// @brief Get the distance to the near plane of the camera.
    ///
    /// @return The distance to the near plane of the camera.
    float getNearDistance() const;

    /// @brief Set the distance to the near plane of the camera.
    ///
    /// @param nearDistance The new distance to the near plane of the camera.
    void setNearDistance(const float nearDistance);

    /// @brief Get the distance to the far plane of the camera.
    ///
    /// @return The distance to the far plane of the camera.
    float getFarDistance() const;

    /// @brief Set the distance to the far plane of the camera.
    ///
    /// @param farDistance The new distance to the far plane of the camera.
    void setFarDistance(const float farDistance);
    
    /// @brief Get the projection matrix of the camera.
    ///
    /// @return The projection matrix of the camera.
    glm::mat4 getProjectionMatrix() const;
    
    /// @brief Get the view-projection matrix of the camera.
    ///
    /// @param viewPoint Position of the camera in world coordinates.
    ///
    /// @return The view-projection matrix of the camera.
    glm::mat4 getViewProjectionMatrix() const;

    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from BasisProvider ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Get the facing direction of the camera.
    ///
    /// @return The direction the camera is facing.
    glm::vec3 forward() const override;

    /// @brief Get the direction to the left of the camera.
    ///
    /// @return The direction to the left of the camera.
    glm::vec3 left() const override;

    /// @brief Get the upwards direction of the camera.
    ///
    /// @return The upwards direction of the camera.
    glm::vec3 up() const override;
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

}//namespace Renderboi

#endif//RENDERBOI__CORE__CAMERA_HPP