#include "shader_config.hpp"

#include <stdexcept>
#include <string>

#include "../../tools/string_tools.hpp"

namespace ShaderInfo
{
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
            config.addFeature(ShaderFeature::FragmentFullColor);
            runOnce = true;
        }

        return config;
    }

    bool ShaderConfig::checkForConflicts(ShaderFeature newFeature)
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
                // If it is, add it to the conflicts array
                _problems.push_back(*kt);
            }
        }

        return !_problems.empty();
    }

    bool ShaderConfig::checkRequirementsAreMet(ShaderFeature newFeature)
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
                // If it is not, add it to the conflicts array
                _problems.push_back(*jt);
            }
        }

        return _problems.empty();
    }

    void ShaderConfig::addFeature(ShaderFeature newFeature)
    {
        if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

        // Check that the new feature would not be incompatible with any already present
        if (checkForConflicts(newFeature))
        {
            std::string s = "ShaderConfig: cannot request new feature "
                            + std::to_string(newFeature)
                            + " as it conflicts with the following already present features: "
                            + StringTools::iterableToString(_problems, ",") + ".";

            throw std::runtime_error(s.c_str());
        }

        // Check that the requirements for the new feature are all met
        if (!checkRequirementsAreMet(newFeature))
        {
            std::string s = "ShaderConfig: cannot request new feature "
                            + std::to_string(newFeature)
                            + " as it requires the following features, which are currently absent from the config: "
                            + StringTools::iterableToString(_problems, ",") + ". Consider using addFeatureWithRequirements.";

            throw std::runtime_error(s.c_str());
        }

        // Add the new feature
        _requestedFeatures.insert(newFeature);
        _configVectorOutdated = true;
    }

    void ShaderConfig::addFeatureWithRequirements(ShaderFeature newFeature)
    {
        if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

        // Check that the new feature would not be incompatible with any already present
        if (checkForConflicts(newFeature))
        {
            std::string s = "ShaderConfig: cannot request new feature "
                            + std::to_string(newFeature)
                            + " as it conflicts with the following already present features: "
                            + StringTools::iterableToString(_problems, ",") + ".";

            throw std::runtime_error(s.c_str());
        }

        // If not all requirements for the new feature are met, add the missing ones
        if (!checkRequirementsAreMet(newFeature))
        {
            for (auto it = _problems.begin(); it != _problems.begin(); it++)
            {
                addFeatureWithRequirements(*it);
            }
        }

        _requestedFeatures.insert(newFeature);
        _configVectorOutdated = true;
    }

    void ShaderConfig::removeFeature(ShaderFeature feature)
    {
        auto it = _requestedFeatures.find(feature);
        if (it != _requestedFeatures.end())
        {
            _requestedFeatures.erase(feature);
            _configVectorOutdated = true;
        }
    }

    const std::vector<ShaderFeature>& ShaderConfig::getRequestedFeatures()
    {
        static std::vector<ShaderFeature> result;
        if (_configVectorOutdated)
        {
            result.clear();
            std::copy(_requestedFeatures.begin(), _requestedFeatures.end(), std::back_inserter(result));
            _configVectorOutdated = false;
        }
        return result;
    }

    const FeatureRequirementsMap& FeatureRequirements()
    {
        static bool runOnce = false;
        static FeatureRequirementsMap map;

        if (!runOnce)
        {
            map[ShaderFeature::VertexMVP]                   = {};
            map[ShaderFeature::VertexNormalsToColor]        = {
                ShaderFeature::VertexMVP
            };
            // map[ShaderFeature::VertexFishEye]               = {};   // IMPLEMENT VERT LENS
            // map[ShaderFeature::GeometryShowNormals]         = {};   // IMPLEMENT GEOM NORMALS
            map[ShaderFeature::FragmentFullColor]           = {};
            map[ShaderFeature::FragmentDepthView]           = {};
            map[ShaderFeature::FragmentPhong]               = {
                ShaderFeature::FragmentMeshMaterial
            };
            map[ShaderFeature::FragmentBlinnPhong]          = {
                ShaderFeature::FragmentMeshMaterial
            };
            // map[ShaderFeature::FragmentFlatShading]         = {};   // IMPLEMENT FRAG FLAT
            map[ShaderFeature::FragmentMeshMaterial]        = {};
            map[ShaderFeature::FragmentBypassVertexColor]   = {};
            map[ShaderFeature::FragmentGammaCorrection]     = {};
            // map[ShaderFeature::FragmentOutline]             = {};   // IMPLEMENT FRAG OUTLINE
            // map[ShaderFeature::FragmentCubemap]             = {};   // IMPLEMENT FRAG CUBEMAP
            // map[ShaderFeature::FragmentBlending]            = {};   // IMPLEMENT FRAG BLENDING
            // map[ShaderFeature::FragmentShadows]             = {};   // IMPLEMENT FRAG SHADOWS

            runOnce = true;
        }

        return map;
    }

    const IncompatibleFeaturesMap& IncompatibleFeatures()
    {
        static bool runOnce = false;
        static IncompatibleFeaturesMap map;

        if (!runOnce)
        {
            map[ShaderFeature::VertexMVP]                   = {};
            map[ShaderFeature::VertexNormalsToColor]        = {};
            // map[ShaderFeature::VertexFishEye]               = {};   // IMPLEMENT VERT LENS
            // map[ShaderFeature::GeometryShowNormals]         = {};   // IMPLEMENT GEOM NORMALS
            map[ShaderFeature::FragmentFullColor]           = {
                ShaderFeature::FragmentDepthView,
                ShaderFeature::FragmentPhong,
                ShaderFeature::FragmentBlinnPhong,
                ShaderFeature::FragmentBypassVertexColor
                // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
            };
            map[ShaderFeature::FragmentDepthView]           = {
                ShaderFeature::FragmentFullColor,
                ShaderFeature::FragmentPhong,
                ShaderFeature::FragmentBlinnPhong,
                ShaderFeature::FragmentBypassVertexColor
                // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
            };
            map[ShaderFeature::FragmentPhong]               = {
                ShaderFeature::FragmentFullColor,
                ShaderFeature::FragmentDepthView,
                ShaderFeature::FragmentBlinnPhong
            };
            map[ShaderFeature::FragmentBlinnPhong]          = {
                ShaderFeature::FragmentFullColor,
                ShaderFeature::FragmentDepthView,
                ShaderFeature::FragmentPhong
            };
            /* map[ShaderFeature::FragmentFlatShading]         = { // IMPLEMENT FRAG FLAT
                ShaderFeature::FragmentFullColor,
                ShaderFeature::FragmentDepthView
            }; */
            map[ShaderFeature::FragmentMeshMaterial]        = {};
            map[ShaderFeature::FragmentBypassVertexColor]   = {
                ShaderFeature::FragmentFullColor,
                ShaderFeature::FragmentDepthView
            };
            map[ShaderFeature::FragmentGammaCorrection]     = {};
            // map[ShaderFeature::FragmentOutline]             = {};   // IMPLEMENT FRAG OUTLINE
            // map[ShaderFeature::FragmentCubemap]             = {};   // IMPLEMENT FRAG CUBEMAP
            // map[ShaderFeature::FragmentBlending]            = {};   // IMPLEMENT FRAG BLENDING
            // map[ShaderFeature::FragmentShadows]             = {};   // IMPLEMENT FRAG SHADOWS

            runOnce = true;
        }

        return map;
    }
}
