/**
    GLTest, metatron_2d_example.cpp
    Purpose: Implementation of class Metatron2DExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 16/06/2019
 */
#include "metatron_2d_example.hpp"
#include "../tools/gl_utils.hpp"

Metatron2DExample::Metatron2DExample() :
    _shader("assets/shaders/mvp.vert", "assets/shaders/vibing_rgb.frag"),
    _position_vbo(0),
    _color_vbo(0),
    _ebo(0),
    _vao(0),
    _vertices{
        0.0f,          0.0f,   0.0f,
        0.0f,          0.5f,   0.0f,        // As indexed in the array:
        SQRT3 / 4.f,   0.25f,  0.0f,		//            7
        SQRT3 / 4.f,  -0.25f,  0.0f,		//  
        0.0f,         -0.5f,   0.0f,		//  12        1        8
        -SQRT3 / 4.f, -0.25f,  0.0f,		//       6         2
        -SQRT3 / 4.f,  0.25f,  0.0f,		//            0
        0.0f,          1.0f,   0.0f,		//       5         3
        SQRT3 / 2.f,   0.5f,   0.0f,		//  11        4        9
        SQRT3 / 2.f,  -0.5f,   0.0f,		//  
        0.0f,         -1.0f,   0.0f,		//            10
        -SQRT3 / 2.f, -0.5f,   0.0f,
        -SQRT3 / 2.f,  0.5f,   0.0f
    },
    _colors{
        WHITE,
        RED,
        MAGENTA,
        BLUE,
        CYAN,
        GREEN,
        YELLOW,
        CYAN,
        GREEN,
        YELLOW,
        RED,
        MAGENTA,
        BLUE
    },
    _indices()
{
}

Metatron2DExample::~Metatron2DExample()
{
}

void Metatron2DExample::setupBuffers()
{
    // Generate a VAO
    glGenVertexArrays(1, &_vao);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // Enable vertex attribute 0 (position)
    glEnableVertexAttribArray(0);

    // Generate a VBO
    glGenBuffers(1, &_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    // Feed colours to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(_colors), _colors, GL_STATIC_DRAW);
    // Set up vertex attribute 1 (colour) pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // Enable vertex attribute 1 (colour)
    glEnableVertexAttribArray(1);

    // Generate an EBO
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    // Feed index sequence to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Set background to black
    glClearColor(BLACK, 1.0f);
}

void Metatron2DExample::unsetBuffers()
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

void Metatron2DExample::computeIndices()
{
    int idx = 0;

    // Iterate 6 times.
    // Each iteration rotates the created lines "clockwise" around the pattern to draw all lines.
    for (int i = 0; i < 6; i++)
    {
        //Indices of key points in the pattern
        int center = 0;                 // Center of the pattern
        int inner = i + 1;              // Point on the inner hexagon
        int nextInner = inner + 1;      // Next point clockwise on the inner hexagon
        if (nextInner == 7)             // Point 6 wraps back to 1 on the inner hexagon
            nextInner = 1;
        int prevInner = inner - 1;      // Previous point clockwise on the inner hexagon
        if (prevInner == 0)             // Point 1 wraps back to 6 on the inner hexagon
            prevInner = 6;
        int nextNextInner = nextInner + 1;
        if (nextNextInner == 7)
            nextNextInner = 1;
        int prevPrevInner = prevInner - 1;
        if (prevPrevInner == 0)
            prevPrevInner = 6;

        int outer = i + 7;              // Point on the outer hexagon
        int nextOuter = outer + 1;      // Next point clockwise on the outer hexagon
        if (nextOuter == 13)            // Point 12 wraps back to 7 on the outer hexagon
            nextOuter = 7;
        int nextNextOuter = nextOuter + 1;
        if (nextNextOuter == 13)
            nextNextOuter = 7;

        _indices[idx++] = outer;
        _indices[idx++] = nextOuter;

        _indices[idx++] = outer;
        _indices[idx++] = nextInner;

        _indices[idx++] = outer;
        _indices[idx++] = nextNextInner;

        _indices[idx++] = outer;
        _indices[idx++] = inner;

        _indices[idx++] = inner;
        _indices[idx++] = nextOuter;

        _indices[idx++] = inner;
        _indices[idx++] = nextInner;

        _indices[idx++] = inner;
        _indices[idx++] = nextNextOuter;

        _indices[idx++] = inner;
        _indices[idx++] = nextNextInner;

        _indices[idx++] = inner;
        _indices[idx++] = center;
    }
}

void Metatron2DExample::run(GLFWwindow* window)
{
    // Fills _indices with indices to dram lines between each vertex in the array declared above
    this->computeIndices();
    this->setupBuffers();

    _shader.use();
    glBindVertexArray(_vao);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(_vao);

        _shader.setFloat("uTime", (float)glfwGetTime());
        glDrawElements(GL_LINES, 108, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwSetWindowShouldClose(window, false);
    this->unsetBuffers();
}