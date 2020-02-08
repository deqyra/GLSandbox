/**
    GLTest, shader.hpp
    Purpose: Shader program handler. Reads source files and compiles and links them.

    @author François Brachais (deqyra)
    @version 1.0 11/09/2019
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader
{
    private:
        unsigned int _id;
        std::unordered_map <std::string, unsigned int> _uniformLocations;

    public:
        Shader(const GLchar* vertexPath = "assets/shaders/default.vert", const GLchar* fragmentPath = "assets/shaders/default.frag");
        ~Shader();

        // Get program ID.
        unsigned int id();

        // Enable the shader.
        void use();

        // Utility uniform functions.
        unsigned int getUniformLocation(const std::string& name);
        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setMat4f(const std::string& name, glm::mat4 value, bool transpose = false);
};

#endif//SHADER_HPP