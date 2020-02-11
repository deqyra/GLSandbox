/**
    GLTest, perspective_triangle_example.cpp
    Purpose: Implementation of class RotatingTexturedCubeExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
 */
#include "rotating_textured_cube_example.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CAMERA_POS glm::vec3(5.f, 3.f, 5.f)

RotatingTexturedCubeExample::RotatingTexturedCubeExample() :
    _shader("assets/shaders/default.vert", "assets/shaders/texture_1ch.frag"),
    _texture("assets/textures/container.jpg", 0),
    _camera(CAMERA_POS, -135.f, -30.f),
    _vbo(0),
    _vao(0),
    _vertices{
        // Positions          // Colors  // Texture2D coords
        -0.5f, -0.5f, -0.5f,  RED,       0.0f, 0.0f,   // Face 1
         0.5f, -0.5f, -0.5f,  GREEN,     1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  BLUE,      1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  CYAN,      1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  MAGENTA,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  YELLOW,    0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  RED,       0.0f, 0.0f,   // Face 2
         0.5f, -0.5f,  0.5f,  GREEN,     1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  BLUE,      1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  CYAN,      1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  MAGENTA,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  YELLOW,    0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  RED,       1.0f, 0.0f,   // Face 3
        -0.5f,  0.5f, -0.5f,  GREEN,     1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  BLUE,      0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  CYAN,      0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  MAGENTA,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  YELLOW,    1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  RED,       1.0f, 0.0f,   // Face 4
         0.5f,  0.5f, -0.5f,  GREEN,     1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  BLUE,      0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  CYAN,      0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  MAGENTA,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  YELLOW,    1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  RED,       0.0f, 1.0f,   // Face 5
         0.5f, -0.5f, -0.5f,  GREEN,     1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  BLUE,      1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  CYAN,      1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  MAGENTA,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  YELLOW,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  RED,       0.0f, 1.0f,   // Face 6
         0.5f,  0.5f, -0.5f,  GREEN,     1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  BLUE,      1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  CYAN,      1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  MAGENTA,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  YELLOW,    0.0f, 1.0f
    },
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(10.f)
{
    _lastTime = (float)glfwGetTime();
}

RotatingTexturedCubeExample::~RotatingTexturedCubeExample()
{
}

void RotatingTexturedCubeExample::setupBuffers()
{
    // Generate a VAO
    glGenVertexArrays(1, &_vao);
    // Once bound, all subsequent VBO and EBO bindings will be stored for proper rebinding
    glBindVertexArray(_vao);

    // Generate a VBO
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // Feed vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // Vertex attribute pointer set-up
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    // Set background to black
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void RotatingTexturedCubeExample::unsetBuffers()
{
    glUseProgram(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
}

void RotatingTexturedCubeExample::run(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    this->setupBuffers();

    _shader.use();
    // Set texture sampler to the bound texture units.
    _shader.setInt("uTexData1", 0);
    glBindVertexArray(_vao);

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    _lastTime = (float)glfwGetTime();

    glm::vec3 cubePos[] = {
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

    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_autoRotate)
        {
            _angle += _speedFactor * (frameTime - _lastTime);
            if (_angle > 360)
            {
                _angle -= 360;
            }
        }

        glm::mat4 transform = glm::mat4(1.0f);
         transform = glm::rotate(transform, glm::radians(_angle), glm::vec3(0.4f, 0.3f, 0.7f));
        //transform = glm::rotate(transform, glm::radians(_angle), glm::vec3(0.f, 0.f, 1.f));

        // Model matrix, transforms all the vertices of all objects about the world origin.
        // Outside this example, a model matrix should be defined on a per-object basis in order for multiple objects to occupy different places in the world.
        // Multiple object locations are simulated further down by drawing the cube with different values of this matrix, updated through a shader uniform.
        glm::mat4 model = glm::mat4(1.0f);
        
        _camera.updateCamera(frameTime - _lastTime);
        glm::mat4 view = _camera.getViewMatrix();

        int dims[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, dims);
        // Projection matrix, adds perspective to the final render.
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(dims[2]) / (float)dims[3], 0.1f, 100.0f);

        _shader.setFloat("uTime", _lastTime);
        _shader.setMat4f("uTransform", transform);
        _shader.setMat4f("uView", view);
        _shader.setMat4f("uProjection", projection);

        int nPos = sizeof(cubePos) / sizeof(glm::vec3);

        for (int i = 0; i < nPos; i++)
        {
            model = glm::translate(glm::mat4(1.0f), cubePos[i]);
            _shader.setMat4f("uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        _lastTime = frameTime;
    }
    glfwSetWindowShouldClose(window, false);

    windowHandler->removeInputProcessor();

    this->unsetBuffers();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
}

void RotatingTexturedCubeExample::handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!_autoRotate)
    {
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            _angle -= 1;
            if (_angle < 0)
            {
                _angle += 360;
            }
        }
        else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            _angle += 1;
            if (_angle > 360)
            {
                _angle -= 360;
            }
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

void RotatingTexturedCubeExample::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    InputProcessor::keyboardCallback(window, key, scancode, action, mods);
    handleKeyboardObjectRotation(window, key, scancode, action, mods);
    _camera.processKeyInput(window, key, scancode, action, mods);
}

void RotatingTexturedCubeExample::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    _camera.processMouseCursor(window, xpos, ypos);
}