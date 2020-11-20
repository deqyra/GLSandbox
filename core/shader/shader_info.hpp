#ifndef CORE__SHADER_INFO_HPP
#define CORE__SHADER_INFO_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "shader_feature.hpp"
#include "shader_stage.hpp"

namespace ShaderInfo
{
    using IncludeDirectiveToFilenameMap = std::unordered_map<std::string, std::string>;

    /// @brief Structure mapping include directives to the source filenames 
    /// which satisfy them.
    extern const IncludeDirectiveToFilenameMap IncludeFilenames;

    /// @brief Get the map describing which source files satisfy a certain 
    /// include directive.
    ///
    /// @return A map describing which source files satisfy a certain include 
    /// directive.
    const IncludeDirectiveToFilenameMap getIncludeFilenames();

    using FilenameToSupportedFeatureMap = std::unordered_map<std::string, std::vector<ShaderFeature>>;

    /// @brief Structure mapping shader source filenames to the include 
    /// shader features which they support.
    extern const FilenameToSupportedFeatureMap SupportedFeatures;
    
    /// @brief Get the map describing which shader features are supported by a 
    /// certain shader source file.
    ///
    /// @return A map describing which shader features are supported by a 
    /// certain shader source file.
    const FilenameToSupportedFeatureMap getSupportedFeatures();
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderFeature v);
}

#endif//CORE__SHADER_INFO_HPP