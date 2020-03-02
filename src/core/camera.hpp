/**
    GLTest, camera.hpp
    Purpose: Define a camera class handling user input to simulate walking and looking around.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>

#include "direction.hpp"
#include "view_projection_provider.hpp"

#include "../include/glad/glad.h"

#define UP glm::vec3(0.f, 1.f, 0.f)

#define YAW -90.f
#define PITCH 0.f
#define ZOOM 45.f

class Camera : public ViewProjectionProvider
{
    private:
        // Camera Attributes
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;
        float _zoom;

        // Euler Angles
        float _yaw;
        float _pitch;

        void updateVectors();

        bool _vectorsUpdated;
        bool _viewUpdated;
        bool _projectionUpdated;

        glm::mat4 _viewMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewProjectionMatrix;

    public:
        Camera(glm::vec3 position, glm::mat4 projection, float yaw = YAW, float pitch = PITCH, glm::vec3 up = UP);
        virtual ~Camera();

        void processMovement(Direction dir, float velocity);
        void processRotation(float yawOffset, float pitchOffset, bool constrainPitch = true);
        void processZoom(float scrollOffset);

        glm::vec3 getPosition();
        void setPosition(glm::vec3 position);
        glm::vec3 getWorldUp();
        void setWorldUp(glm::vec3 up);
        void setProjectionMatrix(glm::mat4 projection);

        virtual glm::mat4 getViewMatrix();
        virtual glm::mat4 getViewMatrix(glm::vec3 position);
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition);
        virtual glm::mat4 getProjectionMatrix();
        virtual glm::mat4 getViewProjectionMatrix();
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

#endif//CAMERA_HPP