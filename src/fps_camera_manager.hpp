/**
    GLTest, fps_camera_manager.hpp
    Purpose: Define a FPS camera manager class able to handle user input to simulate FPS-like behaviour.

    @author Fran�ois Brachais (deqyra)
    @version 1.0 06/02/2020
*/

#ifndef FPS_CAMERA_MANAGER_HPP
#define FPS_CAMERA_MANAGER_HPP

#include <glm/glm.hpp>

#include "core/camera.hpp"
#include "core/direction.hpp"
#include "core/view_projection_provider.hpp"
#include "tools/gl_window.hpp"

#include "../include/glad/glad.h"

#define DIR_INDEX_FORWARD 0
#define DIR_INDEX_BACKWARD 1
#define DIR_INDEX_LEFT 2
#define DIR_INDEX_RIGHT 3

#define SPEED 4.f
#define SPRINT_MUTLIPLIER 1.5f
#define SENSITIVITY 0.1f

class FPSCameraManager : public ViewProjectionProvider
{
    private:
        CameraPtr _camera;

        // Camera options
        float _moveSpeed;
        float _mouseSensitivity;

        // Keyboard handling related stuff
        bool _movement[4];
        bool _sprint;

        // Mouse handling related stuff
        float _lastMouseX;
        float _lastMouseY;
        bool _mouseWasUpdatedOnce;

    public:
        FPSCameraManager(glm::vec3 position, glm::mat4 projection, float yaw = YAW, float pitch = PITCH, glm::vec3 up = UP);
        ~FPSCameraManager();

        void processKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
        void processMouseCursor(GLFWwindow* window, double xpos, double ypos);
        void processMouseScroll(float scrollOffset);

        CameraWPtr getCamera();
        void setCamera(CameraPtr camera);

        void setProjectionMatrix(glm::mat4 projection);
        void updateCamera(float timeDelta);
        glm::vec3 getPosition();

        virtual glm::mat4 getViewMatrix();
        virtual glm::mat4 getViewMatrix(glm::vec3 position);
        virtual glm::vec3 transformWorldPosition(glm::vec3 worldPosition);
        virtual glm::mat4 getProjectionMatrix();
        virtual glm::mat4 getViewProjectionMatrix();
};

#endif//FPS_CAMERA_MANAGER_HPP