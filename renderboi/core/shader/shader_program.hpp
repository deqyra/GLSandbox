#ifndef RENDERBOI__CORE__SHADER__SHADER_PROGRAM_HPP
#define RENDERBOI__CORE__SHADER__SHADER_PROGRAM_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../material.hpp"
#include "shader_info.hpp"

class ShaderBuilder;

/// @brief Handler for a shader program resource on the GPU.
class ShaderProgram
{
    friend ShaderBuilder;

    private:
        /// @param location Location of the shader program resource on the GPU.
        /// @param supportedFeatures Array of literals describing features which
        /// the shader program supports.
        ShaderProgram(unsigned int location, std::vector<ShaderInfo::ShaderFeature> supportedFeatures);

        /// @brief The location of the shader resource on the GPU.
        unsigned int _location;

        /// @brief Array of literals describing features which the shader 
        /// program supports.
        std::vector<ShaderInfo::ShaderFeature> _supportedFeatures;

        using ProgramToUniformLocationMap = std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>>;
        /// @brief Structure mapping uniform locations against their name, and
        /// then against the location of the program they belong to.
        static ProgramToUniformLocationMap _uniformLocations;

        using LocationToRefCountMap = std::unordered_map<unsigned int, unsigned int>;
        /// @brief Structure mapping how many shader instances are referencing a
        /// shader resource on the GPU.
        static LocationToRefCountMap _locationRefCounts;

        /// @brief Free resources upon destroying an instance.
        void cleanup();

    public:
        ShaderProgram(const ShaderProgram& other);
        
        ~ShaderProgram();
        
        ShaderProgram& operator=(const ShaderProgram& other);

        /// @brief Get location of the shader program on the GPU.
        ///
        /// @return The location of the shader program on the GPU.
        unsigned int location() const;

        /// @brief Enable the shader on the GPU.
        void use() const;

        /// @brief Get the GPU location of a named uniform in the program.
        ///
        /// @param name The name of the uniform to locate in the program.
        ///
        /// @return The GPU location of a named uniform in the shader program.
        unsigned int getUniformLocation(const std::string& name) const;

        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type bool.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setBool(const std::string& name, bool value);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type int.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setInt(const std::string& name, int value);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type unsigned int.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setUint(const std::string& name, unsigned int value);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type float.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setFloat(const std::string& name, float value);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type 3-by-3 float matrix.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        /// @param transpose Whether or not to transpose the matrix before
        /// sending.
        void setMat3f(const std::string& name, glm::mat3 value, bool transpose = false);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type 4-by-4 float matrix.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        /// @param transpose Whether or not to transpose the matrix before
        /// sending.
        void setMat4f(const std::string& name, glm::mat4 value, bool transpose = false);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type 3-dimensional float vector.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setVec3f(const std::string& name, glm::vec3 value);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type material.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        void setMaterial(const std::string& name, Material value);
        
        /// @brief Get the features which this shader program supports.
        ///
        /// @return A const reference to an array of literals describing which 
        /// features the shader program supports.
        const std::vector<ShaderInfo::ShaderFeature>& getSupportedFeatures() const;

        /// @brief Tells whether this shader supports a certain feature.
        ///
        /// @param feature Literal describing the feature to check on.
        ///
        /// @return Whether or not the feature is supported.
        bool supports(ShaderInfo::ShaderFeature feature) const;
};

#endif//RENDERBOI__CORE__SHADER__SHADER_PROGRAM_HPP