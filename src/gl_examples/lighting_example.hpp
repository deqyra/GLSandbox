/**
    GLTest, lighting_cube.hpp
    Purpose: Define an example class to display a textured cube with some lighting.

    @author François Brachais (deqyra)
    @version 1.0 06/02/2020
 */
#ifndef LIGHTING_EXAMPLE_HPP
#define LIGHTING_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../core/shader.hpp"
#include "../core/texture_2d.hpp"

#include "../fps_camera_manager.hpp"
#include "../tools/input_processor.hpp"
#include "../gl_example.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/mesh_generator.hpp"

class LightingExample : public GLExample, public InputProcessor
{
    private:
        std::shared_ptr<FPSCameraManager> _camera;

        float _angle;
        bool _autoRotate;
        float _speedFactor;
        float _lastTime;

        void handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods);
        std::shared_ptr<SceneObject> generateSceneMesh(std::shared_ptr<MeshGenerator> generator, Material mat = Material(), Shader shader = Shader());

    public:
        LightingExample();
        virtual ~LightingExample();
        virtual void run(GLFWwindow* window);

        // Overridden InputProcessor callbacks
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif//LIGHTING_EXAMPLE_HPP