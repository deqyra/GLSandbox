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
        std::string _programKey;
        unsigned int _id;

        // Structure storing uniform locations against their name AND the ID of the program they belong to:
        // program ID -> uniform name -> actual location
        static std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>> _uniformLocations;

        // Structure storing program locations against the path of the shaders they were constructed from:
        // program shader names -> program location
        static std::unordered_map<std::string, unsigned int> _programMaps;

        // Structure storing how many shader instances are referencing a shader resource on the GPU:
        // program ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _refCount;

        unsigned int getRefCount();
        unsigned int increaseRefCount();
        unsigned int decreaseRefCount();

    public:
        Shader(const std::string vertexPath = "assets/shaders/mvp.vert", const std::string fragmentPath = "assets/shaders/default.frag");
        Shader(const Shader& other);
        Shader& operator=(const Shader& other);
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
        void setMat3f(const std::string& name, glm::mat3 value, bool transpose = false);
        void setMat4f(const std::string& name, glm::mat4 value, bool transpose = false);
        void setVec3f(const std::string& name, glm::vec3 value);
};

#endif//SHADER_HPP