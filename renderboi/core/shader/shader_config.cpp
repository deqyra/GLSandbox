#include "shader_config.hpp"

#include <stdexcept>
#include <string>

#include <cpptools/string_tools.hpp>

ShaderConfig::ShaderConfig() :
    _requestedFeatures(),
    _problems(),
    _configVectorOutdated(false)
{

}

const ShaderConfig& ShaderConfig::MinimalConfig()
{
    static bool runOnce = false;
    static ShaderConfig config;
    if (!runOnce)
    {
        config.addFeature(ShaderFeature::VertexMVP);
        config.addFeature(ShaderFeature::FragmentFullLight);
        runOnce = true;
    }

    return config;
}

bool ShaderConfig::_checkForConflicts(const ShaderFeature newFeature) const
{
    _problems.clear();

    auto it = IncompatibleFeatures().find(newFeature);
    if (it == IncompatibleFeatures().end()) return false;

    const std::vector<ShaderFeature>& incompatibleFeatures = it->second;

    // For all features incompatible with the newly requested feature
    for (auto jt = incompatibleFeatures.begin(); jt != incompatibleFeatures.end(); jt++)
    {
        // Check if it is part of the currently requested features
        auto kt = _requestedFeatures.find(*jt);
        if (kt != _requestedFeatures.end())
        {
            // If it is, add it to the problems array
            _problems.push_back(*kt);
        }
    }

    return !_problems.empty();
}

bool ShaderConfig::_checkRequirementsAreMet(const ShaderFeature newFeature) const
{
    _problems.clear();

    auto it = FeatureRequirements().find(newFeature);
    if (it == FeatureRequirements().end()) return true;

    const std::vector<ShaderFeature>& featureRequirements = it->second;

    // For all features required by the newly requested feature
    for (auto jt = featureRequirements.begin(); jt != featureRequirements.end(); jt++)
    {
        // Check if it is part of the currently requested features
        auto kt = _requestedFeatures.find(*jt);
        if (kt == _requestedFeatures.end())
        {
            // If it is not, add it to the problems array
            _problems.push_back(*jt);
        }
    }

    return _problems.empty();
}

void ShaderConfig::addFeature(const ShaderFeature newFeature)
{
    if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

    // Check that the new feature would not be incompatible with any already present
    if (_checkForConflicts(newFeature))
    {
        const std::string s = "ShaderConfig: cannot request new feature " + std::to_string(newFeature) + " as it conflicts "
            "with the following already present features: " + StringTools::iterableToString(_problems, ",") + ".";

        throw std::runtime_error(s.c_str());
    }

    // Check that the requirements for the new feature are all met
    if (!_checkRequirementsAreMet(newFeature))
    {
        const std::string s = "ShaderConfig: cannot request new feature " + std::to_string(newFeature)
            + " as it requires the following features, which are currently absent from the config: "
            + StringTools::iterableToString(_problems, ",") + ". Consider using addFeatureWithRequirements.";

        throw std::runtime_error(s.c_str());
    }

    // Add the new feature
    _requestedFeatures.insert(newFeature);
    _configVectorOutdated = true;
}

void ShaderConfig::addFeatureWithRequirements(const ShaderFeature newFeature)
{
    if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

    // Check that the new feature would not be incompatible with any already present
    if (_checkForConflicts(newFeature))
    {
        const std::string s = "ShaderConfig: cannot request new feature " + std::to_string(newFeature) + " as it conflicts "
            "with the following already present features: " + StringTools::iterableToString(_problems, ",") + ".";

        throw std::runtime_error(s.c_str());
    }

    // If not all requirements for the new feature are met, add the missing ones
    if (!_checkRequirementsAreMet(newFeature))
    {
        for (auto it = _problems.begin(); it != _problems.begin(); it++)
        {
            addFeatureWithRequirements(*it);
        }
    }

    _requestedFeatures.insert(newFeature);
    _configVectorOutdated = true;
}

void ShaderConfig::removeFeature(const ShaderFeature feature)
{
    auto it = _requestedFeatures.find(feature);
    if (it != _requestedFeatures.end())
    {
        _requestedFeatures.erase(feature);
        _configVectorOutdated = true;
    }
}

const std::vector<ShaderFeature>& ShaderConfig::getRequestedFeatures() const
{
    static std::vector<ShaderFeature> result;

    if (_configVectorOutdated)
    {
        result.clear();
        std::copy(
            _requestedFeatures.begin(),
            _requestedFeatures.end(),
            std::back_inserter(result)
        );
        
        _configVectorOutdated = false;
    }

    return result;
}

const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& ShaderConfig::FeatureRequirements()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderFeature, std::vector<ShaderFeature>> map;

    if (!runOnce)
    {
        map[ShaderFeature::VertexMVP]                       = {};
        map[ShaderFeature::VertexNormalsToColor]            = {
            ShaderFeature::VertexMVP
        };
        // map[ShaderFeature::VertexFishEye]                   = {};   // IMPLEMENT VERT LENS
        // map[ShaderFeature::GeometryShowNormals]             = {};   // IMPLEMENT GEOM NORMALS
        map[ShaderFeature::FragmentFullLight]               = {};
        map[ShaderFeature::FragmentViewDepthBuffer]         = {};
        map[ShaderFeature::FragmentViewLightAttenuation]    = {};
        map[ShaderFeature::FragmentMeshMaterial]            = {};
        map[ShaderFeature::FragmentBypassVertexColor]       = {};
        map[ShaderFeature::FragmentPhong]                   = {
            ShaderFeature::FragmentMeshMaterial
        };
        map[ShaderFeature::FragmentBlinnPhong]              = {
            ShaderFeature::FragmentMeshMaterial
        };
        // map[ShaderFeature::FragmentFlatShading]             = {};   // IMPLEMENT FRAG FLAT
        map[ShaderFeature::FragmentGammaCorrection]         = {};
        // map[ShaderFeature::FragmentOutline]                 = {};   // IMPLEMENT FRAG OUTLINE
        // map[ShaderFeature::FragmentCubemap]                 = {};   // IMPLEMENT FRAG CUBEMAP
        // map[ShaderFeature::FragmentBlending]                = {};   // IMPLEMENT FRAG BLENDING
        // map[ShaderFeature::FragmentShadows]                 = {};   // IMPLEMENT FRAG SHADOWS

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& ShaderConfig::IncompatibleFeatures()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderFeature, std::vector<ShaderFeature>> map;

    if (!runOnce)
    {
        map[ShaderFeature::VertexMVP]                       = {};
        map[ShaderFeature::VertexNormalsToColor]            = {};
        // map[ShaderFeature::VertexFishEye]                   = {};   // IMPLEMENT VERT LENS
        // map[ShaderFeature::GeometryShowNormals]             = {};   // IMPLEMENT GEOM NORMALS
        map[ShaderFeature::FragmentFullLight]               = {
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        };
        map[ShaderFeature::FragmentViewDepthBuffer]         = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        };
        map[ShaderFeature::FragmentViewLightAttenuation]    = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        };
        map[ShaderFeature::FragmentMeshMaterial]            = {};
        map[ShaderFeature::FragmentBypassVertexColor]       = {};
        map[ShaderFeature::FragmentPhong]                   = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentBlinnPhong
        };
        map[ShaderFeature::FragmentBlinnPhong]              = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong
        };
        /* map[ShaderFeature::FragmentFlatShading]            = { // IMPLEMENT FRAG FLAT
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer
        }; */
        map[ShaderFeature::FragmentGammaCorrection]         = {};
        // map[ShaderFeature::FragmentOutline]                 = {};   // IMPLEMENT FRAG OUTLINE
        // map[ShaderFeature::FragmentCubemap]                 = {};   // IMPLEMENT FRAG CUBEMAP
        // map[ShaderFeature::FragmentBlending]                = {};   // IMPLEMENT FRAG BLENDING
        // map[ShaderFeature::FragmentShadows]                 = {};   // IMPLEMENT FRAG SHADOWS

        runOnce = true;
    }

    return map;
}
