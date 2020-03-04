#ifndef FPS_CAMERA_SCRIPT_HPP
#define FPS_CAMERA_SCRIPT_HPP

#include "../camera.hpp"
#include "../scene/input_processing_script.hpp"

#define DIR_INDEX_FORWARD 0
#define DIR_INDEX_BACKWARD 1
#define DIR_INDEX_LEFT 2
#define DIR_INDEX_RIGHT 3

#define SPEED 4.f
#define SPRINT_MUTLIPLIER 1.5f
#define SENSITIVITY 0.1f

class FPSCameraScript : public InputProcessingScript
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
        FPSCameraScript();

        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        virtual void update(float timeElapsed);

        virtual void setSceneObject(SceneObjectWPtr sceneObject);
};

#endif//FPS_CAMERA_SCRIPT_HPP