/**
    GLTest, perspective_triangle_example.cpp
    Purpose: Implementation of class RotatingTexturedCubeExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
 */
#include "lighting_example.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CAMERA_POS glm::vec3(5.f, 3.f, 5.f)
#define TORUS_ROTATION_AXIS glm::vec3(0.f, 1.f, 0.f)

LightingExample::LightingExample() :
    _shader(),
    _texture("assets/textures/container.jpg", 0),
    _camera(CAMERA_POS, -135.f, -25.f),
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(10.f),
    _torus(2.f, 0.5f, 72, 48),
    _axes(3.f),
    _light(1.f)
{
    _lastTime = (float)glfwGetTime();
    _light.setPosition(glm::vec3(-3.f, 3.f, 0.f));
}

LightingExample::~LightingExample()
{
}

void LightingExample::setupBuffers()
{
    // Set background to black
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void LightingExample::unsetBuffers()
{
    glUseProgram(0);
}

void LightingExample::run(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    this->setupBuffers();

    _shader.use();

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    _lastTime = (float)glfwGetTime();
    
    // Buffer data before drawing meshes
    _torus.setupBuffers();
    _axes.setupBuffers();
    _light.setupBuffers();

    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_autoRotate)
        {
            float angleDiff = _speedFactor * (frameTime - _lastTime);
            _torus.rotate(glm::radians(angleDiff), TORUS_ROTATION_AXIS);
        }
        else
        {
            _torus.lookAt(_camera.getPosition());
        }

        // Projection matrix, adds perspective to the final render.
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), glAspectRatio(), 0.1f, 100.0f);
        _shader.setMat4f("uProjection", projection);

        // View matrix, transpose world coordinates into camera coordinates
        _camera.updateCamera(frameTime - _lastTime);
        glm::mat4 view = _camera.getViewMatrix();
        _shader.setMat4f("uView", view);

        // Draw different meshes, each with its own model matrix, transposing model coordinates into world coordinates
        glm::mat4 model = _torus.getModelMatrix();
        _shader.setMat4f("uModel", model);
        _torus.draw();

        model = _axes.getModelMatrix();
        _shader.setMat4f("uModel", model);
        _axes.draw();

        model = _light.getModelMatrix();
        _shader.setMat4f("uModel", model);
        _light.draw();

        // Refresh screen and process input
        glfwSwapBuffers(window);
        glfwPollEvents();

        _lastTime = frameTime;
    }
    glfwSetWindowShouldClose(window, false);

    windowHandler->removeInputProcessor();

    this->unsetBuffers();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
}

void LightingExample::handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!_autoRotate)
    {
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            _torus.rotate(glm::radians(-1.f), TORUS_ROTATION_AXIS);
        }
        else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            _torus.rotate(glm::radians(1.f), TORUS_ROTATION_AXIS);
        }
    }
    else
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
    _camera.processKeyInput(window, key, scancode, action, mods);
}

void LightingExample::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    _camera.processMouseCursor(window, xpos, ypos);
}