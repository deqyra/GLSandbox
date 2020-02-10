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
#define CUBE_ROTATION_AXIS glm::vec3(0.4f, 0.3f, 0.7f)

LightingExample::LightingExample() :
    _shader(),
    _texture("assets/textures/container.jpg", 0),
    _camera(CAMERA_POS),
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(10.f),
    _cube(1.f),
    _axes(3.f)
{
    _lastTime = (float)glfwGetTime();
}

LightingExample::~LightingExample()
{
}

void LightingExample::setupBuffers()
{
    // Set background to black
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
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
    // Set texture sampler to the bound texture units.
    _shader.setInt("uTexData1", 0);

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    _lastTime = (float)glfwGetTime();

    glm::vec3 positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  0.0f,  0.0f),
        glm::vec3( 4.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  2.0f,  0.0f),
        glm::vec3( 0.0f,  4.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f,  2.0f),
        glm::vec3( 0.0f,  0.0f,  4.0f),
        glm::vec3(-2.0f,  0.0f,  0.0f),
        glm::vec3(-4.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f, -2.0f,  0.0f),
        glm::vec3( 0.0f, -4.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f, -2.0f),
        glm::vec3( 0.0f,  0.0f, -4.0f)
    };
    
    _cube.setupBuffers();
    _axes.setupBuffers();

    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_autoRotate)
        {
            float angleDiff = _speedFactor * (frameTime - _lastTime);
            //_cube.rotate(glm::radians(angleDiff), CUBE_ROTATION_AXIS);
        }
        else
        {
            //_cube.lookAt(_camera.getPosition());
        }

        _camera.updateCamera(frameTime - _lastTime);
        glm::mat4 view = _camera.getViewMatrix();

        int dims[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, dims);
        // Projection matrix, adds perspective to the final render.
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(dims[2]) / (float)dims[3], 0.1f, 100.0f);

        _shader.setFloat("uTime", _lastTime);
        _shader.setMat4f("uView", view);
        _shader.setMat4f("uProjection", projection);

        glm::mat4 model;

        int nPos = sizeof(positions) / sizeof(glm::vec3);
        for (int i = 0; i < nPos; i++)
        {
            _cube.setPosition(positions[i]);
            model = _cube.getModelMatrix();
            _shader.setMat4f("uModel", model);
            _cube.draw();
        }
        model = _axes.getModelMatrix();
        _shader.setMat4f("uModel", model);
        _axes.draw();

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
            _cube.rotate(glm::radians(-1.f), CUBE_ROTATION_AXIS);
        }
        else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            _cube.rotate(glm::radians(1.f), CUBE_ROTATION_AXIS);
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