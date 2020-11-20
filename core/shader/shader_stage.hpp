#ifndef CORE__SHADER__SHADER_STAGE_HPP
#define CORE__SHADER__SHADER_STAGE_HPP

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

    /// @brief Structure mapping shader stages to the macro representing them in
    /// OpenGL.
    extern const ShaderStageToGLMacroMap StageMacros;

    /// @brief Get the map describing which OpenGL macro corresponds to which 
    /// shader stage.
    ///
    /// @return A map describing which OpenGL macro correpsonds to which shader
    /// stage.
    const ShaderStageToGLMacroMap getStageMacros();
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderStage v);
}
#endif//CORE__SHADER__SHADER_STAGE_HPP