#ifndef CORE__SHADER__SHADER_BUILDER_HPP
#define CORE__SHADER__SHADER_BUILDER_HPP

#include <string>

#include "shader.hpp"
#include "shader_config.hpp"

class ShaderBuilder
{
    public:
        static Shader buildFromConfig(ShaderConfig config);
        static Shader buildFromFile(std::string filename);
        static Shader buildFromText(std::string text);
};

#endif//CORE__SHADER__SHADER_BUILDER_HPP