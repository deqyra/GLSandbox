#ifndef CORE__CAMERA_HPP
#define CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "object_transform.hpp"

class Camera
{
    private:
        // Camera facing direction
        glm::vec3 _front;
        // Camera right direction
        glm::vec3 _right;
        // Camera upwards direction
        glm::vec3 _up;
        // Upwards direction of an object the camera might be attached to
        glm::vec3 _parentUp;
        // Rotation between the parent's upwards direction and the world Y vector
        glm::quat _parentUpRotation;
        // Camera zoom factor
        float _zoom;

        // Camera yaw (rotation around Y) in degrees
        float _yaw;
        // Camera yaw (rotation around X) in degrees
        float _pitch;

        // Calera projection matrix (perspective and lens)
        glm::mat4 _projectionMatrix;

        // Recalculate vectors based on yaw, pitch and parent rotation
        void updateVectors();

    public:
        static constexpr float DefaultYaw = -90.f;
        static constexpr float DefaultPitch = 0.f;
        static constexpr float DefaultZoomFactor = 1.f;

        Camera(const Camera& other);
        Camera(glm::mat4 projection, float yaw = DefaultYaw, float pitch = DefaultPitch, float zoom = DefaultZoomFactor, glm::vec3 up = ObjectTransform::Y);
        virtual ~Camera();

        // Update vectors to reflect a new camera rotation
        void processRotation(float yawOffset, float pitchOffset);
        // DO NOT USE - update projection matrix to simulate camera zoom
        void processZoom(float scrollOffset);

        // Get the facing direction of the camera
        glm::vec3 front();
        // Get the right direction of the camera
        glm::vec3 right();
        // Get the upwards direction of the camera
        glm::vec3 up();

        // Get the upwards direction of the camera's parent (if any)
        glm::vec3 getParentUp();
        // Update the registered upwards direction of the camera's parent (if any)
        void setParentUp(glm::vec3 up);
        // Set the projection matrix of the camera
        void setProjectionMatrix(glm::mat4 projection);

        // Get the view matrix of the camera
        virtual glm::mat4 getViewMatrix(glm::vec3 position);
        // Transform a vector from world space to view space
        virtual glm::vec3 worldPositionToViewSpace(glm::vec3 viewPoint, glm::vec3 worldPosition);
        // Get the projection matrix registered for the camera
        virtual glm::mat4 getProjectionMatrix();
        // Get the VP matrix of the camera
        virtual glm::mat4 getViewProjectionMatrix(glm::vec3 viewPos);
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

#endif//CORE__CAMERA_HPP