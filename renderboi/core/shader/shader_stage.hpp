#ifndef RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP
#define RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP

#include <string>
#include <unordered_map>

namespace ShaderInfo
{
    enum class ShaderStage
    {
        Vertex,
        Geometry,
        Fragment
    };

    using ShaderStageToGLMacroMap = std::unordered_map<ShaderStage, unsigned int>;

    /// @brief Get the map describing which OpenGL macro corresponds to which 
    /// shader stage.
    ///
    /// @return A map describing which OpenGL macro correpsonds to which shader
    /// stage.
    const ShaderStageToGLMacroMap& StageMacros();

    using ShaderStageFileExtensionMap = std::unordered_map<ShaderStage, std::string>;

    /// @brief Get the map describing which file extension corresponds to which 
    /// shader stage.
    ///
    /// @return A map describing which file extension corresponds to which 
    /// shader stage.
    const ShaderStageFileExtensionMap& StageFileExtensions();
}

namespace std
{
    string to_string(ShaderInfo::ShaderStage v);
}
#endif//RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP