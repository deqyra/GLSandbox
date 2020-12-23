#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

#include <glad/gl.h>

namespace ShaderInfo
{
    const ShaderStageToGLMacroMap& StageMacros()
    {
        static bool runOnce = false;
        static ShaderStageToGLMacroMap map;

        if (!runOnce)
        {
            map[ShaderStage::Vertex]    = GL_VERTEX_SHADER;
            map[ShaderStage::Geometry]  = GL_GEOMETRY_SHADER;
            map[ShaderStage::Fragment]  = GL_FRAGMENT_SHADER;

            runOnce = true;
        }

        return map;
    }

    const ShaderStageFileExtensionMap& StageFileExtensions()
    {
        static bool runOnce = false;
        static ShaderStageFileExtensionMap map;

        if (!runOnce)
        {
            map[ShaderStage::Vertex]    = "vert";
            map[ShaderStage::Geometry]  = "geom";
            map[ShaderStage::Fragment]  = "frag";

            runOnce = true;
        }

        return map;
    }
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderStage v)
    {
        static bool runOnce = false;
        static std::unordered_map<ShaderInfo::ShaderStage, std::string> _stageNames;

        if (!runOnce)
        {
            _stageNames[ShaderInfo::ShaderStage::Vertex]    = "Vertex";
            _stageNames[ShaderInfo::ShaderStage::Geometry]  = "Geometry";
            _stageNames[ShaderInfo::ShaderStage::Fragment]  = "Fragment";

            runOnce = true;
        }

        auto it = _stageNames.find(v);
        return (it != _stageNames.end()) ?
            it->second : "Unknown shader stage";
    }
}
