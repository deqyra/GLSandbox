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

#include "../core/meshes/cube.hpp"
#include "../core/meshes/axes.hpp"

#include "../fps_camera_manager.hpp"
#include "../tools/input_processor.hpp"
#include "../gl_example.hpp"

class LightingExample : public GLExample, public InputProcessor
{
    private:
        Shader _shader;
        Texture2D _texture;
        FPSCameraManager _camera;

        Cube _cube;
        Axes _axes;

        float _angle;
        bool _autoRotate;
        float _speedFactor;
        float _lastTime;

        void setupBuffers();
        void unsetBuffers();

        void handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:
        LightingExample();
        virtual ~LightingExample();
        virtual void run(GLFWwindow* window);

        // Overridden InputProcessor callbacks
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif//LIGHTING_EXAMPLE_HPP