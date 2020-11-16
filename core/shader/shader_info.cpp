#include "shader_info.hpp"

const ShaderInfo::ShaderToIncludeDirectiveMap ShaderInfo::IncludeDirectives = ShaderInfo::getIncludeRequirements();
const ShaderInfo::IncludeToFilenameMap ShaderInfo::IncludeFilenames = ShaderInfo::getIncludeFilenames();
const ShaderInfo::ShaderToSupportedFeatureMap ShaderInfo::SupportedFeatures = ShaderInfo::getSupportedFeatures();

namespace ShaderInfo
{
    ShaderToIncludeDirectiveMap getIncludeRequirements()
    {
        static bool runOnce = false;
        if (runOnce) return IncludeDirectives;

        ShaderToIncludeDirectiveMap map;

        map["assets/shaders/mvp.vert"]          = {
            "/uniform_blocks/matrices",
            "/interface_blocks/vertex_out"
        };
        map["assets/shaders/phong.frag"]        = {
            "/uniform_blocks/lights",
            "/uniform_blocks/material",
            "/interface_blocks/vertex_out",
            "/functional_blocks/gamma_correction"
        };
        map["assets/shaders/default.frag"]      = {"/interface_blocks/vertex_out"};
        map["assets/shaders/texture_1ch.frag"]  = {"/interface_blocks/vertex_out"};
        map["assets/shaders/texture_2ch.frag"]  = {"/interface_blocks/vertex_out"};
        map["assets/shaders/vibing_rgb.frag"]   = {"/interface_blocks/vertex_out"};

        runOnce = true;
        return map;
    }

    IncludeToFilenameMap getIncludeFilenames()
    {
        static bool runOnce = false;
        if (runOnce) return IncludeFilenames;

        IncludeToFilenameMap map;

        map["/uniform_blocks/matrices"]             = "assets/shaders/uniform_blocks/matrices.glsl";
        map["/uniform_blocks/lights"]               = "assets/shaders/uniform_blocks/lights.glsl";
        map["/uniform_blocks/material"]             = "assets/shaders/uniform_blocks/material.glsl";
        map["/interface_blocks/vertex_out"]         = "assets/shaders/interface_blocks/vertex_out.glsl";
        map["/functional_blocks/gamma_correction"]  = "assets/shaders/functional_blocks/gamma_correction.glsl";

        runOnce = true;
        return map;
    }

    ShaderToSupportedFeatureMap getSupportedFeatures()
    {
        static bool runOnce = false;
        if (runOnce) return SupportedFeatures;

        ShaderToSupportedFeatureMap map;

        map["assets/shaders/phong.frag"]    = {
            ShaderFeature::FragmentMeshMaterial,
            ShaderFeature::FragmentGammaCorrection
        };

        runOnce = true;
        return map;
    }
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderFeature v)
    {
        static std::unordered_map<ShaderInfo::ShaderFeature, std::string> _featureNames;
        static bool runOnce = false;

        if (!runOnce)
        {
            _featureNames[ShaderInfo::ShaderFeature::VertexMVP]                = "VertexMVP";
            _featureNames[ShaderInfo::ShaderFeature::VertexFishEye]            = "VertexFishEye";
            _featureNames[ShaderInfo::ShaderFeature::GeometryShowNormals]      = "GeometryShowNormals";
            _featureNames[ShaderInfo::ShaderFeature::FragmentFullLight]        = "FragmentFullLight";
            _featureNames[ShaderInfo::ShaderFeature::FragmentDepthView]        = "FragmentDepthView";
            _featureNames[ShaderInfo::ShaderFeature::FragmentMeshMaterial]     = "FragmentMeshMaterial";
            _featureNames[ShaderInfo::ShaderFeature::FragmentPhong]            = "FragmentPhong";
            _featureNames[ShaderInfo::ShaderFeature::FragmentBlinnPhong]       = "FragmentBlinnPhong";
            _featureNames[ShaderInfo::ShaderFeature::FragmentGammaCorrection]  = "FragmentGammaCorrection";
            _featureNames[ShaderInfo::ShaderFeature::FragmentOutline]          = "FragmentOutline";
            _featureNames[ShaderInfo::ShaderFeature::FragmentCubemap]          = "FragmentCubemap";
            _featureNames[ShaderInfo::ShaderFeature::FragmentBlending]         = "FragmentBlending";
            _featureNames[ShaderInfo::ShaderFeature::FragmentShadows]          = "FragmentShadows";

            runOnce = true;
        }

        return (_featureNames.find(v) != _featureNames.end()) ?
            _featureNames[v] : "Unknown";
    }
}