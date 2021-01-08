#include "resource_locator.hpp"

#include <filesystem>

namespace Renderboi
{

std::unordered_map<ResourceType, std::filesystem::path>
ResourceLocator::_resourceLocationPrefixes = std::unordered_map<ResourceType, std::filesystem::path>();

void ResourceLocator::setPrefixFor(ResourceType what, std::string prefix)
{
    if (prefix == "")
    {
        _resourceLocationPrefixes.erase(what);
        return;
    }

    _resourceLocationPrefixes[what] = std::filesystem::absolute(prefix);
}

void ResourceLocator::setPrefixFor(ResourceType what, std::filesystem::path prefix)
{
    if (prefix.empty())
    {
        _resourceLocationPrefixes.erase(what);
        return;
    }

    _resourceLocationPrefixes[what] = std::filesystem::absolute(prefix);
}

std::string ResourceLocator::locate(ResourceType type, std::string filename)
{
    auto it = _resourceLocationPrefixes.find(type);
    if (it == _resourceLocationPrefixes.end())
        return filename;
    
    std::filesystem::path file = _resourceLocationPrefixes[type] / std::filesystem::path(filename);
    if (!std::filesystem::exists(file))
    {
        std::string s = "ResourceLocator: cannot locate resource \"" + file.string() + "\".";
        throw std::runtime_error(s.c_str());
    }

    return file.string();
}


}//namespace Renderboi
