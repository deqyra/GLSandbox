#ifndef CORE__SHADER_HPP
#define CORE__SHADER_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "material.hpp"
#include "lights/point_light.hpp"
#include "shader_info.hpp"

/// @brief Handler for a shader resource on the GPU.
class Shader
{
    private:
        /// @brief The paths of the files from which the shader was constructed,
        /// combined together. Used to avoid instantiating the same shader
        /// program several times, taking up unnecessary space on the GPU.
        std::string _programKey;

        /// @brief The location of the shader resource on the GPU.
        unsigned int _location;

        using ShaderToRenderFeatureMap = std::unordered_map<unsigned int, std::unordered_map<typename ShaderInfo::RenderFeature, bool>>;
        /// @brief Structure mapping supported render features to the file names
        /// of the shaders which support them.
        static ShaderToRenderFeatureMap _supportedFeatures;

        using ProgramToUniformLocationMap = std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>>;
        /// @brief Structure mapping uniform locations against their name, and
        /// then against the location of the program they belong to.
        static ProgramToUniformLocationMap _uniformLocations;

        using ProgramKeyToLocationMap = std::unordered_map<std::string, unsigned int>;
        /// @brief Structure mapping program locations against the path of the
        /// shader source files they were constructed from.
        static ProgramKeyToLocationMap _programLocations;

        using LocationToRefCountMap = std::unordered_map<unsigned int, unsigned int>;
        /// @brief Structure mapping how many shader instances are referencing a
        /// shader resource on the GPU.
        static LocationToRefCountMap _locationRefCounts;

        using NamedStringToLoadStatusMap = std::unordered_map<std::string, bool>;  
        /// @brief Structure telling whether a named string was loaded on the 
        /// GPU. Used for shader include directives.
        /// Assume not present == not loaded.
        static NamedStringToLoadStatusMap _namedStringLoadStatus;

        /// @brief Free resources upon destroying an instance.
        void cleanup();

        /// @brief Compile GLSL code from a file into a shader.
        ///
        /// @param shaderType Constant describing which shader stage the 
        /// compiled program will be associated which.
        /// @param filename Local path to the shader source file.
        ///
        /// @return The location of the shader on the GPU, or 0 if the 
        /// compilation failed (error displayed in std::cerr).
        static unsigned int loadShader(unsigned int shaderType, std::string filename);

        /// @brief Process #include directives in a shader source file.
        ///
        /// @param filename Path to the file whose #include directives to 
        /// process.
        static void processIncludeDirectives(std::string filename);

        /// @brief Load the contents of a file into a named string which will be
        /// uploaded to the GPU.
        ///
        /// @param name Name to be assigned to the named string.
        /// @param sourceFilename Path to the source file whose contents to fill
        /// the named string with.
        ///
        /// @exception If the source file cannot be found, the function will 
        /// throw a std::runtime_error.
        static void makeNamedString(std::string name, std::string sourceFilename);

        /// @brief Append to an array the render features supported by a shader.
        ///
        /// @param destination Array to which the features should be appended.
        /// @param filename Path to the shader source file.
        static void appendSupportedFeaturesOfShader(std::vector<ShaderInfo::RenderFeature>& destination, std::string filename);

        /// @brief Combine shaders and link them into a program.
        ///
        /// @param count The number of shaders to link together.
        /// @param ... GPU locations of the shaders to link together. [count]
        /// locations must be provided after the count argument itself, and all
        /// must be of type unsigned int.
        ///
        /// @return The location of the created shader program, or 0 if the 
        /// linking failed (error displayed in std::cerr).
        static unsigned int makeShaderProgram(unsigned int count, ...);

    public:
        /// @param vertexPath Local path to the vertex shader source file.
        /// @param fragmentPath Local path to the fragment shader source file.
        Shader(const std::string vertexPath = "assets/shaders/mvp.vert", const std::string fragmentPath = "assets/shaders/default.frag");
        
        Shader(const Shader& other);
        
        ~Shader();
        
        Shader& operator=(const Shader& other);

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
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type point light.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param value The value to set the uniform at.
        /// @param position The world position of the point light.
        void setPointLight(const std::string& name, PointLight value, glm::vec3 position);
        
        /// @brief Set the value of a named uniform in the program.
        /// Use for uniforms of type array of point lights.
        ///
        /// @param name The name of the uniform whose value to set.
        /// @param index The index of the array cell to set.
        /// @param value The value to set the array cell at.
        /// @param position The world position of the point light.
        void setPointLightArray(const std::string& name, unsigned int index, PointLight value, glm::vec3 position);

        /// @brief Tells whether this shader supports a certain render feature.
        ///
        /// @param feature Literal describing the feature to check on.
        ///
        /// @return Whether or not the feature is supported.
        bool isSupported(ShaderInfo::RenderFeature feature);
};

#endif//CORE__SHADER_HPP