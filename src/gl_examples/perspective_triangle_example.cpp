/**
    GLTest, perspective_triangle_example.cpp
    Purpose: Implementation of class PerspectiveTriangleExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 17/09/2019
 */
#include "perspective_triangle_example.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PerspectiveTriangleExample::PerspectiveTriangleExample() :
    _shader("assets/shaders/default.vert", "assets/shaders/vibing_rgb.frag"),
    _position_vbo(0),
    _color_vbo(0),
    _ebo(0),
    _vao(0),
    _vertices{
        -1.0f, -1.0f, 0.0f,
        -0.5f,  0.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
         0.5f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    },
    _colors{
        RED,
        GREEN,
        BLUE,
        RED,
        GREEN,
        BLUE
    },
    _indices{ 0, 1, 2, 3, 4, 1, 3, 5 },
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(1),
    _time(0)
{
}

PerspectiveTriangleExample::~PerspectiveTriangleExample()
{
}

void PerspectiveTriangleExample::setupBuffers()
{
    // Generate a VAO
    glGenVertexArrays(1, &_vao);
    // Once bound, all subsequent VBO and EBO bindings will be stored for proper rebinding
    glBindVertexArray(_vao);

    // Generate a VBO
    glGenBuffers(1, &_position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _position_vbo);
    // Feed vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // Vertex attribute 0, pointer set-up:
    // - data goes to vertex attribute 0 (position according to our vertex shader)
    // - data comes by packs of 3
    // - data is floating point numbers
    // - data is not normalised
    // - stride is 3 floats wide (0 = tightly packed = [pack size] * [size of type] = 3 in this case)
    // - no offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable vertex attribute 0 (position)
    glEnableVertexAttribArray(0);

    // Generate a VBO
    glGenBuffers(1, &_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    // Feed colours to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(_colors), _colors, GL_STATIC_DRAW);
    // Set up vertex attribute 1 (colour) pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable vertex attribute 1 (colour)
    glEnableVertexAttribArray(1);

    // Generate an EBO
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    // Feed index sequence to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);

    // Set background to black
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

void PerspectiveTriangleExample::unsetBuffers()
{
    glUseProgram(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_position_vbo);
    _position_vbo = 0;
    glDeleteBuffers(1, &_color_vbo);
    _color_vbo = 0;
    glDeleteBuffers(1, &_ebo);
    _ebo = 0;

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
}

void PerspectiveTriangleExample::run(GLFWwindow* window)
{
    this->setupBuffers();

    _shader.use();
    // As the VAO is bound again, the VBO and EBO bindings from setupBuffers() are reinvoked.
    glBindVertexArray(_vao);

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    _time = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (_autoRotate)
        {
            _angle += _speedFactor * ((float)glfwGetTime() - _time);
            if (_angle > 360)
            {
                _angle -= 360;
            }
        }
        _time = (float)glfwGetTime();

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(_angle), glm::vec3(0.f, 0.f, 1.f));

        // Model matrix, transforms all the vertices of all objects (in that case, a couple triangles) about the world origin.
        // Outside this example, a model matrix should be defined on a per-object basis in order for multiple objects to occupy different places in the world.
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // View matrix, transforms all vertices of all objects about the world origin, accounting for a camera placed in the world.
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        int dims[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, dims);
        // Projection matrix, adds perspective to the final render.
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(dims[2]) / (float)dims[3], 0.1f, 100.0f);

        _shader.setFloat("uTime", _time);
        _shader.setMat4f("uTransform", transform);
        _shader.setMat4f("uModel", model);
        _shader.setMat4f("uView", view);
        _shader.setMat4f("uProjection", projection);

        glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, (void*)0);
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * 5));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwSetWindowShouldClose(window, false);

    windowHandler->removeInputProcessor();

    this->unsetBuffers();
}

void PerspectiveTriangleExample::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    InputProcessor::keyboardCallback(window, key, scancode, action, mods);

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
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor < 100)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor > 1)
        {
            _speedFactor /= 1.1f;
        }
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        _autoRotate = !_autoRotate;
    }
}