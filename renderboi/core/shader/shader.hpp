#ifndef CORE__SHADER__SHADER_HPP
#define CORE__SHADER__SHADER_HPP

#include <unordered_map>
#include <vector>

#include "shader_info.hpp"
#include "shader_feature.hpp"
#include "shader_stage.hpp"

class ShaderBuilder;

/// @brief Handler for a shader resource on the GPU.
class Shader
{
    friend ShaderBuilder;

    private:
        /// @param location Location of the shader resource on the GPU.
        /// @param stage Literal describing which shader stage this object is 
        /// for.
        /// @param supportedFeatures Array of literals describing features which
        /// the shader program supports.
        Shader(unsigned int location, ShaderInfo::ShaderStage stage, std::vector<ShaderInfo::ShaderFeature> supportedFeatures);

        /// @brief The location of the shader resource on the GPU.
        unsigned int _location;

        /// @brief Literal describing which shader stage this object is for.
        ShaderInfo::ShaderStage _stage;

        /// @brief Array of literals describing features which the shader 
        /// program supports.
        std::vector<ShaderInfo::ShaderFeature> _supportedFeatures;

        using LocationToRefCountMap = std::unordered_map<unsigned int, unsigned int>;
        /// @brief Structure mapping how many shader instances are referencing a
        /// shader resource on the GPU.
        static LocationToRefCountMap _locationRefCounts;

        /// @brief Free resources upon destroying an instance.
        void cleanup();

    public:
        Shader(const Shader& other);
        
        ~Shader();
        
        Shader& operator=(const Shader& other);

        /// @brief Get location of the shader object on the GPU.
        ///
        /// @return The location of the shader object on the GPU.
        unsigned int location() const;

        /// @brief Get the shader stage of this shader object.
        ///
        /// @return Literal describing the shader stage of this shader object.
        ShaderInfo::ShaderStage stage() const;

        /// @brief Get the features which this shader object supports.
        ///
        /// @return A const reference to an array of literals describing which 
        /// features the shader object supports.
        const std::vector<ShaderInfo::ShaderFeature>& getSupportedFeatures() const;

        /// @brief Tells whether this shader supports a certain feature.
        ///
        /// @param feature Literal describing the feature to check on.
        ///
        /// @return Whether or not the feature is supported.
        bool supports(ShaderInfo::ShaderFeature feature) const;
};

#endif//CORE__SHADER__SHADER_HPP