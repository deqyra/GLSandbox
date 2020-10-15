#ifndef CORE__CAMERA_HPP
#define CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "transform.hpp"

/// @brief A camera managing its own orientation and providing view and projection matrices.
class Camera
{
    private:
        /// @brief Direction the camera is facing.
        glm::vec3 _front;
        /// @brief Direction to the left of the camera.
        glm::vec3 _left;
        /// @brief Upward direction of the camera.
        glm::vec3 _up;
        /// @brief Upwards direction of the object the camera is attached to.
        Transform _parentTransform;
        /// @brief Zoom factor of the camera.
        float _zoom;

        /// @brief Yaw (rotation around Y) of the camera in degrees.
        float _yaw;
        /// @brief Pitch (rotation around X) of the camera in degrees.
        float _pitch;

        /// @brief Projection matrix of the camera. Provides perspective and lens effect.
        glm::mat4 _projectionMatrix;

        /// @brief Recalculate vectors based on yaw, pitch and parent rotation.
        void updateVectors();

    public:
        static constexpr float DefaultYaw = -90.f;
        static constexpr float DefaultPitch = 0.f;
        static constexpr float DefaultZoomFactor = 1.f;

        /// @param projection Projection matrix to use for the camera.
        /// @param yaw Rotation of the camera around Y in degrees.
        /// @param pitch Rotation of the camera around X in degrees.
        /// @param zoom Zoom factor of the camera.
        /// @param parentTransform Transform of the object the camera is attached to.
        Camera(glm::mat4 projection, float yaw = DefaultYaw, float pitch = DefaultPitch, float zoom = DefaultZoomFactor, Transform parentTransform = Transform());

        Camera(const Camera& other);

        /// @brief Update the rotation of the camera to fit recorded offsets in yaw and pitch.
        ///
        /// @param yawOffset How much the camera was rotated around Y in degrees.
        /// @param pitchOffset How much the camera was rotated around X in degrees.
        void processRotation(float yawOffset, float pitchOffset);
        
        /// @brief DO NOT USE - Update the projection matrix of the camera to zoom in or out.
        ///
        /// @param scrollOffset How much the camera was zoomed in or out.
        void processZoom(float scrollOffset);

        /// @brief Get the facing direction of the camera.
        ///
        /// @return The direction the camera is facing.
        glm::vec3 front();

        /// @brief Get the direction to the left of the camera.
        ///
        /// @return The direction to the left of the camera.
        glm::vec3 left();

        /// @brief Get the upwards direction of the camera.
        ///
        /// @return The upwards direction of the camera.
        glm::vec3 up();

        /// @brief Get the world transform of the parent of the camera.
        ///
        /// @return The world transform of the parent of the camera.
        Transform getParentTransform();

        /// @brief Update the registered world transform of the parent of the camera.
        ///
        /// @param transform New world transform of the parent of the camera.
        void setParentTransform(Transform parentTransform);

        /// @brief Set the projection matrix of the camera.
        ///
        /// @param projection New projection matrix of the camera.
        void setProjectionMatrix(glm::mat4 projection);

        /// @brief Get the view matrix of the camera from a certain view point.
        ///
        /// @param viewPoint Position of the camera in world coordinates.
        ///
        /// @return The view matrix of the camera.
        glm::mat4 getViewMatrix(glm::vec3 viewPoint);
        
        /// @brief Transform vector coordinates from world space to view space from a certain view point.
        ///
        /// @param viewPoint Position of the camera in world coordinates.
        /// @param worldPosition Coordinates to transform into view space.
        ///
        /// @return The transformed coordinates.
        glm::vec3 worldPositionToViewSpace(glm::vec3 viewPoint, glm::vec3 worldPosition);
        
        /// @brief Get the projection matrix of the camera.
        ///
        /// @return The projection matrix of the camera.
        glm::mat4 getProjectionMatrix();
        
        /// @brief Get the view-projection matrix of the camera.
        ///
        /// @param viewPoint Position of the camera in world coordinates.
        ///
        /// @return The view-projection matrix of the camera.
        glm::mat4 getViewProjectionMatrix(glm::vec3 viewPoint);
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

#endif//CORE__CAMERA_HPP