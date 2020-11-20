#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

#include <glad/gl.h>

const ShaderInfo::ShaderStageToGLMacroMap ShaderInfo::StageMacros = ShaderInfo::getStageMacros();

namespace ShaderInfo
{
    const ShaderStageToGLMacroMap& getStageMacros()
    {
        static bool runOnce = false;
        if (runOnce) return StageMacros;

        ShaderStageToGLMacroMap map;

        map[ShaderStage::Vertex]    = GL_VERTEX_SHADER;
        map[ShaderStage::Geometry]  = GL_GEOMETRY_SHADER;
        map[ShaderStage::Fragment]  = GL_FRAGMENT_SHADER;

        runOnce = true;
        return map;
    }
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderStage v)
    {
        static std::unordered_map<ShaderInfo::ShaderStage, std::string> _stageNames;
        static bool runOnce = false;

        if (!runOnce)
        {
            _stageNames[ShaderInfo::ShaderStage::Vertex]    = "Vertex";
            _stageNames[ShaderInfo::ShaderStage::Geometry]  = "Geometry";
            _stageNames[ShaderInfo::ShaderStage::Fragment]  = "Fragment";

            runOnce = true;
        }

        auto it = _stageNames.find(v);
        return (it != _stageNames.end()) ?
            it->second : "Unknown";
    }
}
