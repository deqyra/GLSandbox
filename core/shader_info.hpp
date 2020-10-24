#ifndef CORE__SHADER_INFO_HPP
#define CORE__SHADER_INFO_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace ShaderInfo
{
    using ShaderToIncludeDirectiveMap = std::unordered_map<std::string, std::vector<std::string>>;

    /// @brief Structure mapping shader source filenames to the include 
    /// directives which they require.
    extern const ShaderToIncludeDirectiveMap IncludeDirectives;

    /// @brief Whether ShaderInfo::IncludeDirectives was populated. Should 
    /// always be true after program start-up.
    extern bool _includeDirectivessPopulated;
    
    /// @brief Get the map describing which include directives are required
    /// by a certain shader source file.
    ///
    /// @return A map describing which include directives are required by a 
    /// certain shader source file.
    ShaderToIncludeDirectiveMap getIncludeRequirements();

    using IncludeToFilenameMap = std::unordered_map<std::string, std::string>;

    /// @brief Structure mapping include directives to the source filenames 
    /// which satisfy them.
    extern const IncludeToFilenameMap IncludeFilenames;

    /// @brief Whether ShaderInfo::IncludeFilenames was populated. Should 
    /// always be true after program start-up.
    extern bool _includeFilenamesPopulated;

    /// @brief Get the map describing which source files satisfy a certain 
    /// include directive.
    ///
    /// @return A map describing which source files satisfy a certain include 
    /// directive.
    IncludeToFilenameMap getIncludeFilenames();
}

#endif//CORE__SHADER_INFO_HPP