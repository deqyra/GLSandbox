#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

#include <glad/gl.h>

namespace Renderboi
{

std::string to_string(const ShaderStage v)
{
    static bool runOnce = false;
    static std::unordered_map<ShaderStage, std::string> _stageNames;

    if (!runOnce)
    {
        _stageNames[ShaderStage::Vertex]    = "Vertex";
        _stageNames[ShaderStage::Geometry]  = "Geometry";
        _stageNames[ShaderStage::Fragment]  = "Fragment";

        runOnce = true;
    }

    auto it = _stageNames.find(v);
    return (it != _stageNames.end()) ?
        it->second : "Unknown shader stage";
}

}//namespace Renderboi
