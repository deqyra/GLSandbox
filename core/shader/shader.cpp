#include "shader.hpp"

#include <stdexcept>

#include <glad/gl.h>

Shader::LocationToRefCountMap Shader::_locationRefCounts = Shader::LocationToRefCountMap();

Shader::Shader(unsigned int location, ShaderInfo::ShaderStage stage, std::vector<ShaderInfo::ShaderFeature> supportedFeatures) :
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
    cleanup();

    // Copy the location, program key, increase refcount
    _location = other._location;
    _stage = other._stage;
    _locationRefCounts[_location]++;

    return *this;
}

Shader::~Shader()
{
    // Let go of content currently in place
    cleanup();
}

void Shader::cleanup()
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

const std::vector<ShaderInfo::ShaderFeature>& Shader::getSupportedFeatures() const
{
    return _supportedFeatures;
}

bool Shader::supports(ShaderInfo::ShaderFeature feature) const
{
    auto it = std::find(_supportedFeatures.begin(), _supportedFeatures.end(), feature);

    return it != _supportedFeatures.end();
}
