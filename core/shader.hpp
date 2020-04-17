#ifndef CORE__SHADER_HPP
#define CORE__SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "material.hpp"
#include "lights/point_light.hpp"

// Proxy class to a shader resource on the GPU
class Shader
{
    private:
        // The paths of the files from which the shader was constructed, combined together
        std::string _programKey;
        // The location of the shader resource on the GPU
        unsigned int _location;

        // Structure storing uniform locations against their name AND the ID of the program they belong to:
        // program GPU location -> uniform name -> uniform location
        static std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>> _uniformLocations;

        // Structure storing program locations against the path of the shaders they were constructed from:
        // program key -> program GPU location
        static std::unordered_map<std::string, unsigned int> _programMaps;

        // Structure storing how many shader instances are referencing a shader resource on the GPU:
        // program GPU location -> reference count
        static std::unordered_map<unsigned int, unsigned int> _refCount;

        // Free resources
        void cleanup();

        // Compile GLSL code from a file into a shader
        static unsigned int loadShader(unsigned int shaderType, std::string filename);

        // Combine shaders and link them into a program
        static unsigned int makeShaderProgram(unsigned int count...);

    public:
        Shader(const std::string vertexPath = "assets/shaders/mvp.vert", const std::string fragmentPath = "assets/shaders/default.frag");
        Shader(const Shader& other);
        Shader& operator=(const Shader& other);
        ~Shader();

        // Get program location
        unsigned int location() const;

        // Enable the shader
        void use() const;

        // Utility uniform functions
        unsigned int getUniformLocation(const std::string& name) const;

        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setUint(const std::string& name, unsigned int value);
        void setFloat(const std::string& name, float value);
        void setMat3f(const std::string& name, glm::mat3 value, bool transpose = false);
        void setMat4f(const std::string& name, glm::mat4 value, bool transpose = false);
        void setVec3f(const std::string& name, glm::vec3 value);
        void setMaterial(const std::string& name, Material value);
        void setPointLight(const std::string& name, PointLight value, glm::vec3 position);
        void setPointLightArray(const std::string& name, unsigned int index, PointLight value, glm::vec3 position);
};

#endif//CORE__SHADER_HPP