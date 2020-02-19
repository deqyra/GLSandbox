#include "shader_adapter.hpp"

#include <fstream>

#include "../../tools/string_tools.hpp"

std::unordered_map<std::string, UniformDestination> ShaderAdapter::_destinationNames = std::unordered_map<std::string, UniformDestination>();
std::unordered_map<std::string, ShaderConstant> ShaderAdapter::_constantNames = std::unordered_map<std::string, ShaderConstant>();
bool ShaderAdapter::_namesFilled = false;

void ShaderAdapter::parseShaderConfig(const std::string& path)
{
    try
    {
        std::ifstream confFile(path);
        std::string line;
        while (std::getline(confFile, line))
        {
            std::vector<std::string> tokens = StringTools::tokenizeString(line, '=', true);
            if (tokens.size() != 2) continue;

            // Try parsing a uniform destination name
            bool result = parseUniformName(tokens[0], tokens[1]);
            // Try parsing a shader constant name
            if (!result) result = parseShaderConstant(tokens[0], tokens[1]);
            // If both attempts were unsuccessful, throw
            if (!result)
            {
                std::string s = "ShaderAdapter: " + path + ": \"" + tokens[0] + "\": no such uniform destination or shader constant.";
                throw std::runtime_error(s.c_str());
            }
        }
    }
    catch (const std::exception & e)
    {
        return;
    }
}

bool ShaderAdapter::parseUniformName(std::string destinationStr, std::string uniformName)
{
    if (_destinationNames.find(destinationStr) == _destinationNames.end())
    {
        return false;
    }

    UniformDestination dest = _destinationNames[destinationStr];
    _uniformNames[dest] = uniformName;
    return true;
}

bool ShaderAdapter::parseShaderConstant(std::string constantStr, std::string valueStr)
{
    if (_constantNames.find(constantStr) == _constantNames.end())
    {
        return false;
    }
    
    ShaderConstant dest = _constantNames[constantStr];
    try
    {
        int value = std::stoi(valueStr);
        _constantValues[dest] = value;
    }
    catch(const std::exception& e)
    {
        std::string s = "ShaderAdapter: cannot parse \"" + valueStr + "\" to int.";
        throw std::runtime_error(s.c_str());
    }
    return true;
}

void ShaderAdapter::fillNames()
{
    typedef UniformDestination UD;
    typedef ShaderConstant SC;

    _destinationNames["Time"]                     = UD::Time;
    _destinationNames["ModelMatrix"]              = UD::ModelMatrix;
    _destinationNames["ViewMatrix"]               = UD::ViewMatrix;
    _destinationNames["ProjectionMatrix"]         = UD::ProjectionMatrix;
    _destinationNames["NormalMatrix"]             = UD::NormalMatrix;
    _destinationNames["Material"]                 = UD::Material;
    _destinationNames["MaxTextureCount"]          = UD::MaxTextureCount;
    _destinationNames["Texture1Data"]             = UD::Texture1Data;
    _destinationNames["Texture2Data"]             = UD::Texture2Data;
    _destinationNames["TextureMix"]               = UD::TextureMix;
    _destinationNames["PointLightCount"]          = UD::PointLightCount;
    _destinationNames["PointLightArray"]          = UD::PointLightArray;
    _destinationNames["SpotLightCount"]           = UD::SpotLightCount;
    _destinationNames["SpotLightArray"]           = UD::SpotLightArray;
    _destinationNames["DirectionalLightCount"]    = UD::DirectionalLightCount;
    _destinationNames["DirectionalLightArray"]    = UD::DirectionalLightArray;

    _constantNames["MaxPointLightCount"]       = SC::MaxPointLightCount;
    _constantNames["MaxSpotLightCount"]        = SC::MaxSpotLightCount;
    _constantNames["MaxDirectionalLightCount"] = SC::MaxDirectionalLightCount;

    _namesFilled = true;
}

ShaderAdapter::ShaderAdapter(const std::string& vertexPath, const std::string& fragmentPath)
{
    if (!_namesFilled) fillNames();

    parseShaderConfig(vertexPath + ".conf");
    parseShaderConfig(fragmentPath + ".conf");
}

std::string ShaderAdapter::getName(UniformDestination destination)
{
    auto it = _uniformNames.find(destination);
    if (it == _uniformNames.end()) return "";

    return _uniformNames[destination];
}

int ShaderAdapter::getConstant(ShaderConstant constant)
{
    auto it = _constantValues.find(constant);
    if (it == _constantValues.end()) return -1;

    return _constantValues[constant];
}
