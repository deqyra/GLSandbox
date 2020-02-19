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

#include "material.hpp"
#include "shaders/shader_adapter.hpp"
#include "shaders/uniform_destination.hpp"
#include "lights/point_light.hpp"

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

        std::shared_ptr<ShaderAdapter> _adapter;

    public:
        Shader(const std::string vertexPath = "assets/shaders/mvp.vert", const std::string fragmentPath = "assets/shaders/default.frag");
        Shader(const Shader& other);
        Shader& operator=(const Shader& other);
        ~Shader();

        // Get shader adapter.
        std::shared_ptr<ShaderAdapter> getAdapter();

        // Get program ID.
        unsigned int id() const;

        // Enable the shader.
        void use() const;

        // Utility uniform functions.
        unsigned int getUniformLocation(const std::string& name) const;

        unsigned int getUint(const std::string& name);
        int getConstant(ShaderConstant constant);

        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setUint(const std::string& name, unsigned int value);
        void setFloat(const std::string& name, float value);
        void setMat3f(const std::string& name, glm::mat3 value, bool transpose = false);
        void setMat4f(const std::string& name, glm::mat4 value, bool transpose = false);
        void setVec3f(const std::string& name, glm::vec3 value);
        void setMaterial(const std::string& name, Material value);
        void setPointLight(const std::string& name, PointLight value);
        void setPointLightArray(const std::string& name, unsigned int index, PointLight value);

        void setBool(UniformDestination dest, bool value);
        void setInt(UniformDestination dest, int value);
        void setUint(UniformDestination dest, unsigned int value);
        void setFloat(UniformDestination dest, float value);
        void setMat3f(UniformDestination dest, glm::mat3 value, bool transpose = false);
        void setMat4f(UniformDestination dest, glm::mat4 value, bool transpose = false);
        void setVec3f(UniformDestination dest, glm::vec3 value);
        void setMaterial(UniformDestination dest, Material value);
        void setPointLight(UniformDestination dest, PointLight value);
        void setPointLightArray(UniformDestination dest, unsigned int index, PointLight value);
};

#endif//SHADER_HPP