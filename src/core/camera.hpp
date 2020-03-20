#ifndef CORE__CAMERA_HPP
#define CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>

#define UP glm::vec3(0.f, 1.f, 0.f)

#define YAW -90.f
#define PITCH 0.f
#define ZOOM 45.f

class Camera
{
    private:
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;
        float _zoom;

        float _yaw;
        float _pitch;

        void updateVectors();

        glm::mat4 _projectionMatrix;

    public:
        Camera(glm::mat4 projection, float yaw = YAW, float pitch = PITCH, glm::vec3 up = UP);
        virtual ~Camera();

        void processRotation(float yawOffset, float pitchOffset, bool constrainPitch = true);
        void processZoom(float scrollOffset);

        glm::vec3 front();
        glm::vec3 right();
        glm::vec3 up();

        glm::vec3 getWorldUp();
        void setWorldUp(glm::vec3 up);
        void setProjectionMatrix(glm::mat4 projection);

        virtual glm::mat4 getViewMatrix(glm::vec3 position);
        virtual glm::vec3 transformWorldPosition(glm::vec3 viewPos, glm::vec3 worldPosition);
        virtual glm::mat4 getProjectionMatrix();
        virtual glm::mat4 getViewProjectionMatrix(glm::vec3 viewPos);
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

#endif//CORE__CAMERA_HPP