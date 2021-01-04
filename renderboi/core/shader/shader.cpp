#include "shader.hpp"

#include <algorithm>
#include <stdexcept>

#include <glad/gl.h>

namespace Renderboi
{

std::unordered_map<unsigned int, unsigned int> Shader::_locationRefCounts = std::unordered_map<unsigned int, unsigned int>();

Shader::Shader(unsigned int location, ShaderStage stage, const std::vector<ShaderFeature> supportedFeatures) :
    _location(location),
    _stage(stage),
    _supportedFeatures(supportedFeatures)
{
    if (!location)
    {
        throw std::runtime_error("Shader: cannot create object wrapping no resource on the GPU (location == 0).");
    }

    auto it = _locationRefCounts.find(location);
    if (it == _locationRefCounts.end()) _locationRefCounts[location] = 0;

    _locationRefCounts[location]++;
}

Shader::Shader(const Shader& other) :
    _location(other._location),
    _stage(other._stage),
    _supportedFeatures(other._supportedFeatures)
{
    _locationRefCounts[_location]++;
}

Shader& Shader::operator=(const Shader& other)
{
    // Let go of content currently in place
    _cleanup();

    // Copy the location, program key, increase refcount
    _location = other._location;
    _stage = other._stage;
    _locationRefCounts[_location]++;

    return *this;
}

Shader::~Shader()
{
    // Let go of content currently in place
    _cleanup();
}

void Shader::_cleanup()
{
    // Decrease the ref count
    unsigned int count = --_locationRefCounts[_location];
    // If refcount is zero, destroy resource on the GPU
    if (!count)
    {
        glDeleteShader(_location);
    };
}

unsigned int Shader::location() const
{
    return _location;
}

ShaderStage Shader::stage() const
{
    return _stage;
}

const std::vector<ShaderFeature>& Shader::getSupportedFeatures() const
{
    return _supportedFeatures;
}

bool Shader::supports(const ShaderFeature feature) const
{
    auto it = std::find(
        _supportedFeatures.begin(),
        _supportedFeatures.end(),
        feature
    );

    return it != _supportedFeatures.end();
}

}//namespace Renderboi
