/**
    GLTest, texture_example.cpp
    Purpose: Implementation of class Texture2DExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 13/09/2019
 */
#include "texture_2d_example.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include <exception>
#include <iostream>

using std::runtime_error;

Texture2DExample::Texture2DExample() :
    _shader("assets/shaders/mvp.vert", "assets/shaders/texture_2ch.frag"),
    _texture("assets/textures/container.jpg", 0),
    _overlay("assets/textures/awesomeface.png", 1, true, true),
    _vbo(0),
    _ebo(0),
    _vao(0),
    _vertices{
        // Positions            // Colors   // Texture2D coords
         0.5f,  0.5f, 0.0f,     RED,        1.0f, 1.0f,     // Top right
         0.5f, -0.5f, 0.0f,     GREEN,      1.0f, 0.0f,     // Bottom right
        -0.5f, -0.5f, 0.0f,     BLUE,       0.0f, 0.0f,     // Bottom left
        -0.5f,  0.5f, 0.0f,     YELLOW,     0.0f, 1.0f      // Top left 
    },
    _indices{ 1, 0, 2, 3 },
    _mixValue(0.3f)
{

}

Texture2DExample::~Texture2DExample()
{

}

void Texture2DExample::setupBuffers()
{
    // Generate a VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Generate a VBO
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // Feed vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // Setup vertex attribute pointers.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Enable vertex attributes.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Generate an EBO
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    // Feed index sequence to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Set background to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Texture2DExample::unsetBuffers()
{
    glUseProgram(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;
    glDeleteBuffers(1, &_ebo);
    _ebo = 0;

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
}

void Texture2DExample::run(GLFWwindow* window)
{
    this->setupBuffers();

    _shader.use();
    _texture.bind();
    _overlay.bind();

    // Set texture samplers to their respectively bound texture units.
    _shader.setInt("uTexData1", 0);
    _shader.setInt("uTexData2", 1);
    _shader.setFloat("uTextureMix", _mixValue);

    glBindVertexArray(_vao);

    // Retrieve the custom window pointer, register this example as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(this);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwSetWindowShouldClose(window, false);

    // Remove the input processor from the window
    windowHandler->removeInputProcessor();

    this->unsetBuffers();
}

void Texture2DExample::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Keep the basic stuff working
    InputProcessor::keyboardCallback(window, key, scancode, action, mods);
    
    // Make the awesome face more visible upon pressing the up arrow
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (_mixValue < 1.f)
        {
            _mixValue += 0.01f;
            if (_mixValue > 1.f)
            {
                _mixValue = 1.f;
            }
            _shader.setFloat("uTextureMix", _mixValue);
        }
    }
    // Make it less visible when pressing the down arrow
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (_mixValue > 0.f)
        {
            _mixValue -= 0.01f;
            if (_mixValue < 0.f)
            {
                _mixValue = 0.f;
            }
            _shader.setFloat("uTextureMix", _mixValue);
        }
    }
}