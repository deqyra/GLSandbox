#include "resource_locator.hpp"

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
    
    return (_resourceLocationPrefixes[type] / std::filesystem::path(filename)).string();
}


}//namespace Renderboi
