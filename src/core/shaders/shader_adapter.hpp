#ifndef SHADER_ADAPTER_HPP
#define SHADER_ADAPTER_HPP

#include <string>
#include <unordered_map>

#include "uniform_destination.hpp"
#include "shader_constant.hpp"

class ShaderAdapter
{
    private:
        std::unordered_map<UniformDestination, std::string> _uniformNames;
        std::unordered_map<ShaderConstant, int> _constantValues;
        void parseShaderConfig(const std::string& path);
        bool parseUniformName(std::string destinationStr, std::string uniformName);
        bool parseShaderConstant(std::string constantStr, std::string valueStr);

        static std::unordered_map<std::string, UniformDestination> _destinationNames;
        static std::unordered_map<std::string, ShaderConstant> _constantNames;
        static bool _namesFilled;
        static void fillNames();

    public:
        ShaderAdapter(const std::string& vertexPath, const std::string& fragmentPath);
        std::string getName(UniformDestination destination);
        int getConstant(ShaderConstant constant);
};

#endif//SHADER_ADAPTER_HPP