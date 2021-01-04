#ifndef RENDERBOI__UTILITIES__RESOURCE_LOCATOR_HPP
#define RENDERBOI__UTILITIES__RESOURCE_LOCATOR_HPP

#include <string>
#include <filesystem>
#include <unordered_map>

namespace Renderboi
{

enum class ResourceType
{
    ShaderSource,
    Texture
};

/// @brief Eases out finding the correct full path to a ressource.
class ResourceLocator
{
private:
    /// @brief Structure mapping path prefixes to the resource type they lead to.
    static std::unordered_map<ResourceType, std::filesystem::path> _resourceLocationPrefixes;

public:
    /// @brief Set the path prefix of a certain resource type. Provide prefix
    /// "" to remove entry.
    ///
    /// @param what Resource type to assign the path prefix to.
    /// @param prefix Path prefix to assign to the resource type.
    static void setPrefixFor(ResourceType what, std::string prefix);

    /// @brief Set the path prefix of a certain resource type. Provide prefix
    /// "" to remove entry.
    ///
    /// @param what Resource type to assign the path prefix to.
    /// @param prefix Path prefix to assign to the resource type.
    static void setPrefixFor(ResourceType what, std::filesystem::path prefix);

    /// @brief Retrieve the full path to a certain resource, given its type and
    /// filename.
    ///
    /// @param type Type of the resource to locate.
    /// @param filename Incomplete path to the ressource.
    ///
    /// @return The full path to the resource.
    static std::string locate(ResourceType type, std::string filename);
};

}//namespace Renderboi


#endif//RENDERBOI__UTILITIES__RESOURCE_LOCATOR_HPP