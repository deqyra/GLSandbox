#include "shader.hpp"

#include "../tools/gl_utils.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

using std::endl;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::unordered_map;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    // Compile and link provided shaders
    unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
    if (!vertexShader)
    {
        stringstream errorText;
        errorText << "Shader error: \"" << vertexPath << "\" is not valid vertex shader." << endl;
        throw new runtime_error(errorText.str().c_str());
    }

    unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
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
}

Shader::~Shader()
{
    glDeleteProgram(_id);
}

// Get program ID.
unsigned int Shader::id()
{
    return _id;
}

// Enable the shader.
void Shader::use()
{
    glUseProgram(_id);
}

// Utility uniform functions.
unsigned int Shader::getUniformLocation(const string& name)
{
    // First check in the location hash map if the uniform location is not ther.
    auto it = _uniformLocations.find(name);
    if (it != _uniformLocations.end())
    {
        // If the location is present, return it.
        return it->second;
    }

    // Otherwise, retrieve the location and store it away before returning it.
    unsigned int location = glGetUniformLocation(_id, name.c_str());
    if (location)
    {
        _uniformLocations[name] = location;
    }
    return location;
}

void Shader::setBool(const string& name, bool value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glUniform1i(uniformLocation, (int)value);
}

void Shader::setInt(const string& name, int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glUniform1i(uniformLocation, value);
}

void Shader::setFloat(const string& name, float value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glUniform1f(uniformLocation, value);
}

void Shader::setMat4f(const string& name, glm::mat4 value, bool transpose)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glUniformMatrix4fv(uniformLocation, 1, transposition, glm::value_ptr(value));
}
