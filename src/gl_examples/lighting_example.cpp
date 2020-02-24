/**
    GLTest, perspective_triangle_example.cpp
    Purpose: Implementation of class RotatingTexturedCubeExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
 */
#include "lighting_example.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include "../core/meshes/torus.hpp"
#include "../core/meshes/axes.hpp"
#include "../core/meshes/cube.hpp"
#include "../core/lights/point_light.hpp"
#include "../core/materials.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#define CAMERA_POS glm::vec3(5.f, 3.f, 5.f)
#define CUBE_ROTATION_AXIS glm::vec3(0.f, 1.f, 0.f)
#define TORUS_ROTATION_AXIS glm::vec3(1.f, 0.f, 0.f)

LightingExample::LightingExample() :
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(10.f)
{
    _camera = std::make_shared<FPSCameraManager>(CAMERA_POS, -135.f, -25.f),
    _lastTime = (float)glfwGetTime();
}

LightingExample::~LightingExample()
{
}

void LightingExample::run(GLFWwindow* window)
{
    // Remove cursor from window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    // Set background to black
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    _lastTime = (float)glfwGetTime();
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    // Instantiate scene meshes
    std::shared_ptr<Torus> torus = std::make_shared<Torus>(2.f, 0.5f, 72, 48);
    std::shared_ptr<Axes> axes = std::make_shared<Axes>(3.f);
    std::shared_ptr<Cube> lightCube = std::make_shared<Cube>(1.f);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>();

    // Setup mesh properties
    torus->material = Materials::Emerald;

    // Setup light
    glm::vec3 lightPosition = glm::vec3(-3.f, 3.f, 0.f);
    lightCube->setPosition(lightPosition);
    light->setPosition(lightPosition);

    // Register mesh in mesh drawer
    MeshDrawer meshDrawer = MeshDrawer();
    meshDrawer.registerMesh(torus, lightingShader);
    meshDrawer.registerMesh(axes);
    meshDrawer.registerMesh(lightCube);
    meshDrawer.registerLight(light);

    // Register camera in mesh drawer
    meshDrawer.setCamera(_camera);

    // Setup projection in mesh drawer
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), glAspectRatio(), 0.1f, 100.0f);
    meshDrawer.setProjection(projection);

    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera
        _camera->updateCamera(frameTime - _lastTime);

        if (_autoRotate)
        {
            // Update object transforms
            float angleDiff = _speedFactor * (frameTime - _lastTime);
            lightCube->orbit((float)glm::radians(0.618 * angleDiff), CUBE_ROTATION_AXIS, glm::vec3(0.f, 3.f, 0.f));
            light->orbit((float)glm::radians(0.618 * angleDiff), CUBE_ROTATION_AXIS, glm::vec3(0.f, 3.f, 0.f));
            torus->rotate((float)glm::radians(angleDiff), TORUS_ROTATION_AXIS);

            // Update light position in lighting shader
            glm::vec3 lightPos = light->getPosition();
            glm::vec3 lightViewPos = _camera->transformWorldPosition(lightPos);
        }
        else
        {
            //torus->lookAt(_camera->getPosition());
        }

        // Draw meshes
        meshDrawer.drawMeshes();

        // Refresh screen and process input
        glfwSwapBuffers(window);
        glfwPollEvents();

        _lastTime = frameTime;
    }
    glfwSetWindowShouldClose(window, false);

    windowHandler->removeInputProcessor();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
}

void LightingExample::handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (_autoRotate)
    {
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor < 200)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor > 10)
        {
            _speedFactor /= 1.1f;
        }
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        _autoRotate = !_autoRotate;
    }
}

void LightingExample::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    InputProcessor::keyboardCallback(window, key, scancode, action, mods);
    handleKeyboardObjectRotation(window, key, scancode, action, mods);
    _camera->processKeyInput(window, key, scancode, action, mods);
}

void LightingExample::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    _camera->processMouseCursor(window, xpos, ypos);
}