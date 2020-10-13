#ifndef CORE__CAMERA_HPP
#define CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "object_transform.hpp"

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
        glm::vec3 _parentUp;
        /// @brief Rotation of the parent within the world.
        glm::quat _parentRotation;
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

        Camera(const Camera& other);
        Camera(glm::mat4 projection, float yaw = DefaultYaw, float pitch = DefaultPitch, float zoom = DefaultZoomFactor, glm::vec3 up = ObjectTransform::Y);
        ~Camera();

        /// @brief Update the rotation of the camera to fit recorded offsets in yaw and pitch.
        ///
        /// @param[in] yawOffset How much the camera was rotated around Y in degrees.
        /// @param[in] pitchOffset How much the camera was rotated around X in degrees.
        void processRotation(float yawOffset, float pitchOffset);
        
        /// @brief DO NOT USE - Update the projection matrix of the camera to zoom in or out.
        ///
        /// @param[in] scrollOffset How much the camera was zoomed in or out.
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

        /// @brief Get the upwards direction of the parent of the camera.
        ///
        /// @return The upwards direction of the parent of the camera.
        glm::vec3 getParentUp();

        /// @brief Update the registered upwards direction of the parent of the camera.
        ///
        /// @param[in] up New upwards direction of the parent of the camera.
        void setParentUp(glm::vec3 up);

        /// @brief Set the projection matrix of the camera.
        ///
        /// @param[in] projection New projection matrix of the camera.
        void setProjectionMatrix(glm::mat4 projection);

        /// @brief Get the view matrix of the camera from a certain view point.
        ///
        /// @param[in] viewPoint Position of the camera in world coordinates.
        ///
        /// @return The view matrix of the camera.
        glm::mat4 getViewMatrix(glm::vec3 viewPoint);
        
        /// @brief Transform vector coordinates from world space to view space from a certain view point.
        ///
        /// @param[in] viewPoint Position of the camera in world coordinates.
        /// @param[in] worldPosition Coordinates to transform into view space.
        ///
        /// @return The transformed coordinates.
        glm::vec3 worldPositionToViewSpace(glm::vec3 viewPoint, glm::vec3 worldPosition);
        
        /// @brief Get the projection matrix of the camera.
        ///
        /// @return The projection matrix of the camera.
        glm::mat4 getProjectionMatrix();
        
        /// @brief Get the view-projection matrix of the camera.
        ///
        /// @param[in] viewPoint Position of the camera in world coordinates.
        ///
        /// @return The view-projection matrix of the camera.
        glm::mat4 getViewProjectionMatrix(glm::vec3 viewPoint);
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

#endif//CORE__CAMERA_HPP