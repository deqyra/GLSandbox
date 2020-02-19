/**
    GLTest, shader.cpp
    Purpose: Implementation of class Shader. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 11/09/2019
*/

#include "shader.hpp"

#include "../tools/gl_utils.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <filesystem>

using std::endl;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::unordered_map;

namespace fs = std::filesystem;

unordered_map<unsigned int, unordered_map<string, unsigned int>> Shader::_uniformLocations = unordered_map<unsigned int, unordered_map<string, unsigned int>>();
unordered_map<string, unsigned int> Shader::_programMaps = unordered_map<string, unsigned int>();
unordered_map<unsigned int, unsigned int> Shader::_refCount = unordered_map<unsigned int, unsigned int>();

Shader::Shader(const string vertexPath, const string fragmentPath)
{
    _programKey = fs::absolute(vertexPath).string() + fs::absolute(fragmentPath).string();

    auto it = _programMaps.find(_programKey);
    if (it == _programMaps.end())
    {
        // Compile and link provided shaders
        unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, (const GLchar*) vertexPath.c_str());
        if (!vertexShader)
        {
            stringstream errorText;
            errorText << "Shader error: \"" << vertexPath << "\" is not valid vertex shader." << endl;
            throw new runtime_error(errorText.str().c_str());
        }

        unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, (const GLchar*) fragmentPath.c_str());
        if (!fragmentShader)
        {
            stringstream errorText;
            errorText << "Shader error: \"" << fragmentPath << "\" is not a valid fragment shader." << endl;
            throw new runtime_error(errorText.str().c_str());
        }

        _id = makeShaderProgram(2, vertexShader, fragmentShader);
        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!_id)
        {
            throw new runtime_error("Shader error: provided shaders could not be assembled into a program.");
        }

        _programMaps[_programKey] = _id;
    }
    else
    {
        _id = _programMaps[_programKey];
    }

    increaseRefCount();
}

Shader::Shader(const Shader& other)
{
    _id = other._id;
    increaseRefCount();
}

Shader& Shader::operator=(const Shader& other)
{
    _id = other._id;
    increaseRefCount();

    return *this;
}


Shader::~Shader()
{
    // Decrease the ref count
    // If empty, destroy resource
    if (decreaseRefCount() == 0)
    {
        _programMaps.erase(_programKey);
        glDeleteProgram(_id);
    }
}

// Get program ID.
unsigned int Shader::id() const
{
    return _id;
}

// Enable the shader.
void Shader::use() const
{
    glUseProgram(_id);
}

// Utility uniform functions.
unsigned int Shader::getUniformLocation(const string& name) const
{
    // First find the program ID in the location hash map.
    auto it = _uniformLocations.find(_id);
    if (it != _uniformLocations.end())
    {
        // If the program ID is present, attempt to find the uniform location.
        auto jt = _uniformLocations[_id].find(name);
        if (jt != _uniformLocations[_id].end())
        {
            return jt->second;
        }
    }
    else
    {
        // If the program ID is not present, put an empty hash map in place.
        _uniformLocations[_id] = unordered_map<string, unsigned int>();
    }

    // If the location could not be found, retrieve the location and store it away before returning it.
    unsigned int location = glGetUniformLocation(_id, name.c_str());
    if (location != -1)
    {
        _uniformLocations[_id][name] = location;
    }
    return location;
}

void Shader::setBool(const string& name, bool value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_id, uniformLocation, (int)value);
}

void Shader::setInt(const string& name, int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_id, uniformLocation, value);
}

void Shader::setFloat(const string& name, float value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1f(_id, uniformLocation, value);
}

void Shader::setMat3f(const string& name, glm::mat3 value, bool transpose)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix3fv(_id, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void Shader::setMat4f(const string& name, glm::mat4 value, bool transpose)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix4fv(_id, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void Shader::setVec3f(const std::string& name, glm::vec3 value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform3f(_id, uniformLocation, value.x, value.y, value.z);
}

void Shader::setMaterial(const std::string& name, Material value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    setVec3f(name + ".ambient", value.ambient);
    setVec3f(name + ".diffuse", value.diffuse);
    setVec3f(name + ".specular", value.specular);
    setFloat(name + ".shininess", value.shininess);
}

unsigned int Shader::getRefCount()
{
    auto it = _refCount.find(_id);
    // No check on whether the iterator is valid, as the element MUST exist
    return it->second;
}

unsigned int Shader::increaseRefCount()
{
    auto it = _refCount.find(_id);
    // If resource was not found, set ref count to 1
    if (it == _refCount.end())
    {
        _refCount[_id] = 1;
        return 1;
    }
    // Otherwise, increment it
    else return ++(it->second);
}

unsigned int Shader::decreaseRefCount()
{
    auto it = _refCount.find(_id);
    // No check on whether the iterator is valid, as the element MUST exist
    return --(it->second);
}