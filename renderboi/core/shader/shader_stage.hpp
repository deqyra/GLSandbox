#ifndef RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP
#define RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP

#include <string>
#include <unordered_map>

enum class ShaderStage
{
    Vertex,
    Geometry,
    Fragment
};

namespace std
{
    string to_string(const ShaderStage v);
}
#endif//RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP