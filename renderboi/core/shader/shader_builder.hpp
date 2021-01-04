#ifndef RENDERBOI__CORE__SHADER__SHADER_BUILDER_HPP
#define RENDERBOI__CORE__SHADER__SHADER_BUILDER_HPP

#include <string>

#include "shader.hpp"
#include "shader_program.hpp"
#include "shader_stage.hpp"
#include "shader_config.hpp"

namespace Renderboi
{

class ShaderBuilder
{
public:
    /// @brief Version of GLSL used by ShaderBuilder.
    static constexpr unsigned int ShadingLanguageVersion = 420;

    /// @brief Profile of GLSL used by ShaderBuilder.
    static constexpr char ShadingLanguageProfile[] = "core";

    /// @brief Build a shader program corresponding to the minimal 
    /// functionality which should be provided by a shader program.
    ///
    /// @return A ShaderProgram object wrapping resources on the GPU.
    static ShaderProgram MinimalShaderProgram();

    /// @brief Build shader stages from an expected configuration, link them
    /// together and return a ShaderProgram instance wrapping the resulting
    /// resource on the GPU.
    ///
    /// @param config Object describing the requested features which the
    /// built shader should support.
    ///
    /// @return A ShaderProgram object wrapping resources on the GPU.
    static ShaderProgram BuildShaderProgramFromConfig(const ShaderConfig& config, const bool dumpSource = false);

    /// @brief Combine shaders and link them into a program.
    ///
    /// @param shaders Array of shader instances wrapping the GPU shader 
    /// objects to link together.
    ///
    /// @return A ShaderProgram object wrapping resources on the GPU.
    ///
    /// @exception If more than one shader per stage is provided, the 
    /// function will throw an std::runtime_error. Both a vertex shader and
    /// a fragment shader are required; if either is missing, the function 
    /// will throw an std::runtime_error. If the provided shaders could not
    /// be linked  together, the function will throw an std::runtime_error. 
    /// Additionnally, error info will be printed to std::cerr.
    static ShaderProgram LinkShaders(const std::vector<Shader>& shaders);

    /// @brief Build a shader stage of the given type from an expected 
    /// configuration and return its location on the GPU.
    ///
    /// @param stage Literal describing the shader stage for which the
    /// source code will be compiled.
    /// @param config Object describing the requested features which the
    /// built shader should support.
    /// 
    /// @note Any requested config entries provided in config which do not
    /// match the provided shader type will be ignored.
    ///
    /// @return A Shader object wrapping resources on the GPU.
    static Shader BuildShaderStageFromConfig(const ShaderStage stage, const ShaderConfig& config, const bool dumpSource = false);

    /// @brief Build a shader stage of the given type from the given source
    /// file and return its location on the GPU.
    ///
    /// @param stage Literal describing the shader stage for which the
    /// source code will be compiled.
    /// @param filename Path to the file containing the GLSL source code to
    /// compile.
    /// @param supportedFeatures An array of literals describing the 
    /// features which the shader stage will support. If empty or not 
    /// provided, the function will look up information about the file from
    /// which the shader stage should be built.
    ///
    /// @note A shader program built from several shader stages supports the
    /// features which come from all different stages. If a shader stage 
    /// comes with no info regarding its supported features, all shaders 
    /// programs constructed from it will not provide support for any of its
    /// potential supported features, regardless of whether it actually does.
    ///
    /// @return A Shader object wrapping resources on the GPU.
    ///
    /// @exception If any invalid #include directives are present in the 
    /// source code, the function will throw an std::runtime_error.
    static Shader BuildShaderStageFromFile(
        const ShaderStage stage,
        const std::string& filename,
        const std::vector<ShaderFeature>& supportedFeatures = {}
    );

    /// @brief Build a shader stage of the given type from the given source
    /// code and return its location on the GPU.
    ///
    /// @param stage Literal describing the shader stage for which the
    /// source code will be compiled.
    /// @param text GLSL source code to compile.
    /// @param supportedFeatures An array of literals describing the 
    /// features which the shader stage will support. If empty or not 
    /// provided, the function will look up information about the file from
    /// which the shader stage should be built.
    ///
    /// @note A shader program built from several shader stages supports the
    /// features which come from all different stages. If a shader stage 
    /// comes with no info regarding its supported features, all shaders 
    /// programs constructed from it will not provide support for any of its
    /// potential supported features, regardless of whether it actually does.
    ///
    /// @return A Shader object wrapping resources on the GPU.
    ///
    /// @exception If any invalid #include directives are present in the 
    /// source code, the function will throw an std::runtime_error.
    static Shader BuildShaderStageFromText(
        const ShaderStage stage,
        const std::string& text,
        const std::vector<ShaderFeature>& supportedFeatures,
        const bool dumpSource = false
    );

private:
    /// @brief Structure telling whether a named string was loaded on the 
    /// GPU. Used for shader #include directives.
    /// Assume that (string not present == string not loaded).
    static std::unordered_map<std::string, bool> _namedStringLoadStatus;

    /// @brief Combine shaders and link them into a program.
    ///
    /// @param locations GPU locations of the shaders to link together.
    ///
    /// @return The location of the created shader program, or 0 if the 
    /// linking failed (error displayed in std::cerr).
    static unsigned int _MakeShaderProgram(const std::vector<unsigned int>& locations);

    /// @brief Process #include directives in shader source code. Generates
    /// named strings corresponding to the found #include directives and 
    /// sends them to OpenGL.
    ///
    /// @param text Source code to process.
    ///
    /// @exception If an #include directive is found to be badly formatted,
    /// the function will throw an std::runtime_error.
    static void _ProcessIncludeDirectives(const std::string& text);

    /// @brief Find arguments to valid #include directives in shader source 
    /// code.
    ///
    /// @param text Source code to process.
    ///
    /// @return An array filled with filenames used in valid #include
    /// directives.
    ///
    /// @exception If an #include directive is found to be badly formatted,
    /// the function will throw an std::runtime_error.
    static std::unordered_set<std::string> _FindIncludeDirectivesInSource(std::string text, const bool recursive);

    /// @brief Load the contents of a file into a named string which will be
    /// uploaded to the GPU.
    ///
    /// @param name Name to be assigned to the named string.
    /// @param sourceFilename Path to the source file whose contents to fill
    /// the named string with.
    ///
    /// @exception If the source file cannot be found, the function will 
    /// throw a std::runtime_error.
    static void _MakeNamedString(const std::string& name, const std::string& sourceFilename);

    /// @brief Append the features supported by a shader to an array.
    ///
    /// @param shaders Array of shader instances wrapping the GPU shader 
    /// objects to link together.
    ///
    /// @return An array filled with all the different features which the 
    /// provided shader objects support.
    static std::vector<ShaderFeature> _AggregateShaderFeatures(const std::vector<Shader>& shaders);

    /// @brief Filter features in provided array by the shader stage they
    /// belong to, and get an array of the features which match the provided
    /// stage.
    ///
    /// @param features Set of features to filter.
    /// @param stage Shader stage to filter features on.
    ///
    /// @return An array with only the features which matched the requested
    /// shader stage.
    static std::vector<ShaderFeature> _FilterFeaturesByStage(const std::vector<ShaderFeature>& features, const ShaderStage stage);

    /// @brief Write source code to hard disk and return filename.
    ///
    /// @param stage Literal describing the shader stage which the source 
    /// code was meant for.
    /// @param text Source code to dump.
    ///
    /// @return Path to the file which the source was dumped to.
    static std::string _DumpShaderSource(const ShaderStage stage, const std::string& text);

    /// @brief Get a string containing the appropriate GLSL version 
    /// directive.
    ///
    /// @return A string containing the appropriate GLSL version directive.
    static const std::string& _GenerateVersionDirective();

    /// @brief Get a string containing the appropriate GLSL extension 
    /// directives.
    ///
    /// @return A string containing the appropriate GLSL extensions
    /// directives.
    static const std::string& _GenerateExtensionDirectives();

    /// @brief Get a structure holding all extensions which may be used by 
    /// shaders at some point.
    ///
    /// @return A map holding all extensions which may be used by shaders at
    /// some point.
    static const std::unordered_map<std::string, std::string>& _ShadingLanguageExtensions();

    /// @brief Structure holding the paths to the templates to generate 
    /// shader stages from.
    ///
    /// @return A map holding the paths to the templates to generate 
    /// shader stages from.
    static const std::unordered_map<ShaderStage, std::string>& _StageTemplatePaths();

    /// @brief Structure holding the macros to define in a template in order
    /// to make use of a given feature.
    ///
    /// @return A map holding the macros to define in a template in order
    /// to make use of a given feature.
    static const std::unordered_map<ShaderFeature, std::string>& _FeatureDefineMacros();

    /// @brief Get the map describing which source files satisfy a certain 
    /// include directive.
    ///
    /// @return A map describing which source files satisfy a certain include 
    /// directive.
    static const std::unordered_map<std::string, std::string>& _IncludeFilenames();

    /// @brief Get the map describing which shader features are supported by a 
    /// certain shader source file.
    ///
    /// @return A map describing which shader features are supported by a 
    /// certain shader source file.
    static const std::unordered_map<std::string, std::vector<ShaderFeature>>& _FeaturesSupportedByFile();

    /// @brief Get the map describing which OpenGL macro corresponds to which 
    /// shader stage.
    ///
    /// @return A map describing which OpenGL macro correpsonds to which shader
    /// stage.
    static const std::unordered_map<ShaderStage, unsigned int>& _ShaderStageMacros();

    /// @brief Get the map describing which file extension corresponds to which 
    /// shader stage.
    ///
    /// @return A map describing which file extension corresponds to which 
    /// shader stage.
    static const std::unordered_map<ShaderStage, std::string>& _StageFileExtensions();

    /// @brief Generate the #define directives to include at the top of a 
    /// shader source in order for it to make use of certain features.
    ///
    /// @param features Array containing literals describing the features
    /// for which to generate #define directives.
    ///
    /// @return A string containing all the necessary #define directives.
    static std::string _GenerateDefineDirectives(const std::vector<ShaderFeature>& features);
};

}//namespace Renderboi

#endif//RENDERBOI__CORE__SHADER__SHADER_BUILDER_HPP