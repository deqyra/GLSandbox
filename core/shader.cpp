#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdarg>
#include <unordered_map>

#include "ubo/ubo_info.hpp"
#include "gl_macros.hpp"

#define INFO_BUFFER_SIZE 512

namespace fs = std::filesystem;

std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>> Shader::_uniformLocations = std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>>();
std::unordered_map<std::string, unsigned int> Shader::_programMaps = std::unordered_map<std::string, unsigned int>();
std::unordered_map<unsigned int, unsigned int> Shader::_refCount = std::unordered_map<unsigned int, unsigned int>();

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
    // Concatenate string paths to get the program key
    _programKey = fs::absolute(vertexPath).string() + fs::absolute(fragmentPath).string();

    auto it = _programMaps.find(_programKey);
    // If the program key is not being used by a shader resource...
    if (it == _programMaps.end())
    {
        // Compile provided shaders
        unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, (const GLchar*) vertexPath.c_str());
        if (!vertexShader)
        {
            std::stringstream errorText;
            errorText << "Shader error: \"" << vertexPath << "\" is not valid vertex shader." << std::endl;
            throw std::runtime_error(errorText.str().c_str());
        }

        unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, (const GLchar*) fragmentPath.c_str());
        if (!fragmentShader)
        {
            std::stringstream errorText;
            errorText << "Shader error: \"" << fragmentPath << "\" is not a valid fragment shader." << std::endl;
            throw std::runtime_error(errorText.str().c_str());
        }

        // Link compiled shaders
        _location = makeShaderProgram(2, vertexShader, fragmentShader);

        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!_location)
        {
            throw std::runtime_error("Shader error: provided shaders could not be assembled into a program.");
        }

        // Map the resource GPU location to the program key
        _programMaps[_programKey] = _location;
        // Set the refcount
        _refCount[_location] = 1;
    }
    // If the program key is already being used by a shader resource...
    else
    {
        // Just copy the GPU location and increase the refcount
        _location = _programMaps[_programKey];
        _refCount[_location]++;
    }
}

Shader::Shader(const Shader& other)
{
    // Copy the location, program key, increase refcount
    _location = other._location;
    _programKey = other._programKey;
    _refCount[_location]++;
}

Shader& Shader::operator=(const Shader& other)
{
    // Let go of content currently in place
    cleanup();

    // Copy the location, program key, increase refcount
    _location = other._location;
    _programKey = other._programKey;
    _refCount[_location]++;

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
    unsigned int count = --_refCount[_location];
    // If refcount is zero, destroy resource on the GPU
    if (!count)
    {
        _programMaps.erase(_programKey);
        glDeleteProgram(_location);
    };
}

unsigned int Shader::location() const
{
    return _location;
}

void Shader::use() const
{
    glUseProgram(_location);
}

unsigned int Shader::getUniformLocation(const std::string& name) const
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

    // If the location could not be found, retrieve the location and store it away before returning it
    int location = glGetUniformLocation(_location, name.c_str());
    if (location != -1)
    {
        _uniformLocations[_location][name] = location;
    }
    return location;
}

void Shader::setBool(const std::string& name, bool value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_location, uniformLocation, (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1i(_location, uniformLocation, value);
}

void Shader::setUint(const std::string& name, unsigned int value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1ui(_location, uniformLocation, value);
}

void Shader::setFloat(const std::string& name, float value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform1f(_location, uniformLocation, value);
}

void Shader::setMat3f(const std::string& name, glm::mat3 value, bool transpose)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix3fv(_location, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void Shader::setMat4f(const std::string& name, glm::mat4 value, bool transpose)
{
    unsigned int transposition = GL_FALSE;
    if (transpose)
    {
        transposition = GL_TRUE;
    }

    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniformMatrix4fv(_location, uniformLocation, 1, transposition, glm::value_ptr(value));
}

void Shader::setVec3f(const std::string& name, glm::vec3 value)
{
    unsigned int uniformLocation = getUniformLocation(name);
    glProgramUniform3fv(_location, uniformLocation, 1, glm::value_ptr(value));
}

void Shader::setMaterial(const std::string& name, Material value)
{
    unsigned int uniformLocation = getUniformLocation(name);
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

void Shader::setPointLight(const std::string& name, PointLight value, glm::vec3 position)
{
    unsigned int uniformLocation = getUniformLocation(name);
    setVec3f(name + ".position", position);
    setVec3f(name + ".ambient", value.ambient);
    setVec3f(name + ".diffuse", value.diffuse);
    setVec3f(name + ".specular", value.specular);
}

void Shader::setPointLightArray(const std::string& name, unsigned int index, PointLight value, glm::vec3 position)
{
    std::string indexedName = name + "[" + std::to_string(index) + "]";
    setPointLight(indexedName, value, position);
}

unsigned int Shader::loadShader(unsigned int shaderType, std::string filename)
{
	// Open input file and read all of its contents.
	std::ifstream file(filename);

	if (!file.is_open())
		std::cerr << "Shader \"" << filename << "\" could not be found." << std::endl;

	std::string all("");
	std::string line;
	while (std::getline(file, line))
		all += line + "\n";

	const char* source = all.c_str();

	// Compile into shader
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, INFO_BUFFER_SIZE, nullptr, info);
		std::cerr << "Shader compilation \"" << filename << "\" failed:\n" << info << std::endl;
		return 0;
	}

	return shader;
}

// There must be `count` arguments after `count`, all of type `unsigned int`.
unsigned int Shader::makeShaderProgram(unsigned int count...)
{
	unsigned int program = glCreateProgram();

	// Iterate through arguments.
	va_list args;
	va_start(args, count);

	for (unsigned int i = 0; i < count; i++)
	{
		// Attach all provided shaders
		unsigned int shader = va_arg(args, unsigned int);
		glAttachShader(program, shader);
	}

	va_end(args);

	// Link all shaders
	glLinkProgram(program);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info);
		std::cerr << "Shader linking failed:\n" << info << std::endl;
		return 0;
	}

	return program;
}

