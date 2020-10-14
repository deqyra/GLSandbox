#include "axes_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../vertex.hpp"

#include "../common_macros.hpp"

AxesGenerator::AxesGenerator() :
    AxesGenerator(DefaultAxisLength)
{

}

AxesGenerator::AxesGenerator(float size) : 
    parameters{ size }
{

}

AxesGenerator::AxesGenerator(Parameters parameters) :
    parameters(parameters)
{

}

MeshPtr AxesGenerator::generatePtr()
{
    float len = parameters.axisLength;

    std::vector<Vertex> vertices = {
        // Position                     // Color            // Normal                       // Tex coord
        {  glm::vec3(0.f, 0.f, 0.f),    glm::vec3(RED),     glm::vec3(0.f, -1.f,  0.f),     glm::vec2(0.f) },   // Vertex 1
        {  glm::vec3(len, 0.f, 0.f),    glm::vec3(RED),     glm::vec3(0.f, -1.f,  0.f),     glm::vec2(0.f) },   // Vertex 2
        {  glm::vec3(0.f, 0.f, 0.f),    glm::vec3(GREEN),   glm::vec3(0.f,  0.f, -1.f),     glm::vec2(0.f) },   // ...
        {  glm::vec3(0.f, len, 0.f),    glm::vec3(GREEN),   glm::vec3(0.f,  0.f, -1.f),     glm::vec2(0.f) },
        {  glm::vec3(0.f, 0.f, 0.f),    glm::vec3(BLUE),    glm::vec3(-1.f, 0.f,  0.f),     glm::vec2(0.f) },
        {  glm::vec3(0.f, 0.f, len),    glm::vec3(BLUE),    glm::vec3(-1.f, 0.f,  0.f),     glm::vec2(0.f) }
    };

    std::vector<unsigned int> indices = {
        0, 1,   // X axis
        2, 3,   // Y axis
        4, 5    // Z axis
    };

    return std::make_shared<Mesh>(vertices, indices, GL_LINES);
}
