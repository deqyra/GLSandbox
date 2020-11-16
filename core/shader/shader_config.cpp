#include "shader_config.hpp"

#include <stdexcept>
#include <string>

#include "../../tools/string_tools.hpp"

namespace ShaderInfo
{
    bool ShaderConfig::checkForConflicts(ShaderFeature newFeature)
    {
        _detectedConflicts.clear();

        auto it = IncompatibleFeatures.find(newFeature);
        if (it == IncompatibleFeatures.end()) return false;

        const std::vector<ShaderFeature>& incompatibleFeatures = it->second;

        // For all features incompatible with the newly requested feature
        for (auto jt = incompatibleFeatures.begin(); jt != incompatibleFeatures.end(); jt++)
        {
            // Check if it is part of the currently requested features
            auto kt = _requestedFeatures.find(*jt);
            if (kt != _requestedFeatures.end() && kt->second)
            {
                // If it is, add it to the conflicts array
                _detectedConflicts.push_back(kt->first);
            }
        }

        return !_detectedConflicts.empty();
    }

    void ShaderConfig::addFeature(ShaderFeature newFeature)
    {
        if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

        if (checkForConflicts(newFeature))
        {
            std::string s = "ShaderConfig: cannot request new feature "
                            + std::to_string(newFeature)
                            + " as it conflicts with the following already present features: "
                            + StringTools::iterableToString(_detectedConflicts, ",") + ".";

            throw std::runtime_error(s.c_str());
        }

        _requestedFeatures[newFeature] = true;
    }

    void ShaderConfig::removeFeature(ShaderFeature feature)
    {
        _requestedFeatures.erase(feature);
    }

    std::unordered_map<ShaderFeature, bool> ShaderConfig::getRequestedFeatures()
    {
        return _requestedFeatures;
    }

    const IncompatibleFeaturesMap IncompatibleFeatures = getIncompatibleFeatures();

    IncompatibleFeaturesMap getIncompatibleFeatures()
    {
        static bool runOnce = false;
        if (runOnce) return IncompatibleFeatures;

        IncompatibleFeaturesMap map;

        map[ShaderFeature::VertexMVP]                   = {};
        map[ShaderFeature::VertexFishEye]               = {};
        map[ShaderFeature::GeometryShowNormals]         = {};
        map[ShaderFeature::FragmentFullLight]           = {
            ShaderFeature::FragmentDepthView,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
        };
        map[ShaderFeature::FragmentDepthView]           = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
        };
        map[ShaderFeature::FragmentPhong]               = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentDepthView,
            ShaderFeature::FragmentBlinnPhong
        };
        map[ShaderFeature::FragmentBlinnPhong]          = {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentDepthView,
            ShaderFeature::FragmentPhong
        };
        map[ShaderFeature::FragmentMeshMaterial]        = {};
        map[ShaderFeature::FragmentGammaCorrection]     = {};
        map[ShaderFeature::FragmentOutline]             = {};
        map[ShaderFeature::FragmentCubemap]             = {};
        map[ShaderFeature::FragmentBlending]            = {};
        map[ShaderFeature::FragmentShadows]             = {};

        runOnce = true;
        return map;
    }
}
