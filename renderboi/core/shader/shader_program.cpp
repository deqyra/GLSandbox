#include "shader_program.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "shader_feature.hpp"

std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>>
ShaderProgram::_uniformLocations = std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>>();

std::unordered_map<unsigned int, unsigned int>
ShaderProgram::_locationRefCounts = std::unordered_map<unsigned int, unsigned int>();

ShaderProgram::ShaderProgram(const unsigned int location, const std::vector<ShaderFeature> supportedFeatures) :
    _location(location),
    _supportedFeatures(supportedFeatures)
{
    if (!location)
    {
        throw std::runtime_error("ShaderProgram: cannot create object wrapping no resource on the GPU (location == 0).");
    }

    auto it = _locationRefCounts.find(location);
    if (it == _locationRefCounts.end()) _locationRefCounts[location] = 0;

    _locationRefCounts[location]++;
}

ShaderProgram::ShaderProgram(const ShaderProgram& other)
{
    // Copy the location, program key, increase refcount
    _location = other._location;
    _supportedFeatures = other._supportedFeatures;
    _locationRefCounts[_location]++;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other)
{
    // Let go of content currently in place
    _cleanup();

    // Copy the location, program key, increase refcount
    _location = other._location;
    _supportedFeatures = other._supportedFeatures;
    _locationRefCounts[_location]++;

    return *this;
}

ShaderProgram::~ShaderProgram()
{
    // Let go of content currently in place
    _cleanup();
}

void ShaderProgram::_cleanup()
{
    // Decrease the ref count
    unsigned int count = --_locationRefCounts[_location];
    // If refcount is zero, destroy resource on the GPU
    if (!count)
    {
        _uniformLocations.erase(_location);
        glDeleteProgram(_location);
    };
}

unsigned int ShaderProgram::location() const
{
    return _location;
}

void ShaderProgram::use() const
{
    glUseProgram(_location);
}

unsigned int ShaderProgram::getUniformLocation(const std::string& name) const
{
    // First find the program ID in the location hash map
    auto it = _uniformLocations.find(_location);
    if (it != _uniformLocations.end())
    {
        // If the program ID is present, attempt to find the uniform location
        auto jt = _uniformLocations[_location].find(name);
        if (jt != _uniformLocations[_location].end())
        {
            return jt->second;
        }
    }
    else
    {
        // If the program ID is not present, put an empty hash map in place
        _uniformLocations[_location] = std::unordered_map<std::string, unsigned int>();
    }

    // If the location was found, store it away before returning it
    int location = glGetUniformLocation(_location, name.c_str());
    if (location != -1)
    {
        _uniformLocations[_location][name] = location;
    }
    // Otherwise, print a warning message
    else
    {
        std::cerr << "ShaderProgram: attempt to get location of uniform \"" << name << "\", "
            "which does not exist in shader program with location \"" << _location << "\"" << std::endl;
    }
    
    return location;
}

void ShaderProgram::setBool(const std::string& name, const bool value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_location, uniformLocation, (int)value);
}

void ShaderProgram::setInt(const std::string& name, const int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_location, uniformLocation, value);
}

void ShaderProgram::setUint(const std::string& name, const unsigned int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1ui(_location, uniformLocation, value);
}

void ShaderProgram::setFloat(const std::string& name, const float value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1f(_location, uniformLocation, value);
}

void ShaderProgram::setMat3f(
    const std::string& name,
    const glm::mat3& value,
    const bool transpose
)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix3fv(_location, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void ShaderProgram::setMat4f(
    const std::string& name,
    const glm::mat4& value,
    const bool transpose
)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix4fv(_location, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void ShaderProgram::setVec3f(const std::string& name, const glm::vec3& value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform3fv(_location, uniformLocation, 1, glm::value_ptr(value));
}

void ShaderProgram::setMaterial(const std::string& name, const Material& value)
{
    setVec3f(name + ".ambient", value.ambient);
    setVec3f(name + ".diffuse", value.diffuse);
    setVec3f(name + ".specular", value.specular);
    setFloat(name + ".shininess", value.shininess);

    unsigned int count = value.getDiffuseMapCount();
    // Diffuse maps are bound in texture units 0 through 7
    for (unsigned int i = 0; i < count; i++)
    {
        std::string samplerName = name + ".diffuseMaps[" + std::to_string(i) + "]";
        setInt(samplerName, (int)i);
    }
    setUint(name + ".diffuseMapCount", count);

    count = value.getSpecularMapCount();
    // Specular maps are bound in texture units 8 through 15
    for (unsigned int i = 0; i < count; i++)
    {
        std::string samplerName = name + ".specularMaps[" + std::to_string(i) + "]";
        setInt(samplerName, Material::SpecularMapMaxCount + (int)i);
    }
    setUint(name + ".specularMapCount", count);
}

const std::vector<ShaderFeature>& ShaderProgram::getSupportedFeatures() const
{
    return _supportedFeatures;
}

bool ShaderProgram::supports(const ShaderFeature feature) const
{
    auto it = std::find(
        _supportedFeatures.begin(),
        _supportedFeatures.end(),
        feature
    );

    return it != _supportedFeatures.end();
}
