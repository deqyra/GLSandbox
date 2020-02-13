/**
    GLTest, camera.hpp
    Purpose: Define a camera class handling user input to simulate walking and looking around.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

#include "../include/glad/glad.h"
#include "directions.hpp"
#include "view_provider.hpp"

#define UP glm::vec3(0.f, 1.f, 0.f)

#define YAW -90.f
#define PITCH 0.f
#define ZOOM 45.f

class Camera : public ViewProvider
{
    private:
        // Camera Attributes
        glm::vec3 _pos;
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

        glm::mat4 _viewMatrix;

    public:
        Camera(glm::vec3 position, float yaw = YAW, float pitch = PITCH, glm::vec3 up = UP);

        glm::mat4 getViewMatrix();
        glm::vec3 getPosition();

        void processMovement(Direction dir, float velocity);
        void processRotation(float yawOffset, float pitchOffset, bool constrainPitch = true);
        void processZoom(float scrollOffset);
};

#endif//CAMERA_HPP