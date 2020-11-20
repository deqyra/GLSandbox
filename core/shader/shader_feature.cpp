#include "shader_feature.hpp"

#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

namespace ShaderInfo
{
    const ShaderFeatureToShaderStageMap FeatureStages = getFeatureStages();

    const ShaderFeatureToShaderStageMap getFeatureStages()
    {
        static bool runOnce = false;
        if (runOnce) return FeatureStages;

        ShaderFeatureToShaderStageMap map;

        map[ShaderFeature::VertexMVP]                   = ShaderStage::Vertex;
        map[ShaderFeature::VertexNormalsToColor]        = ShaderStage::Vertex;
        // map[ShaderFeature::VertexFishEye]               = ShaderStage::Vertex;      // IMPLEMENT VERT LENS
        // map[ShaderFeature::GeometryShowNormals]         = ShaderStage::Geometry;    // IMPLEMENT GEOM NORMALS
        map[ShaderFeature::FragmentFullColor]           = ShaderStage::Fragment;
        map[ShaderFeature::FragmentDepthView]           = ShaderStage::Fragment;
        map[ShaderFeature::FragmentMeshMaterial]        = ShaderStage::Fragment;
        map[ShaderFeature::FragmentBypassVertexColor]   = ShaderStage::Fragment;
        // map[ShaderFeature::FragmentFlatShading]         = ShaderStage::Fragment;    // IMPLEMENT FRAG FLAT
        map[ShaderFeature::FragmentPhong]               = ShaderStage::Fragment;
        map[ShaderFeature::FragmentBlinnPhong]          = ShaderStage::Fragment;
        map[ShaderFeature::FragmentGammaCorrection]     = ShaderStage::Fragment;
        // map[ShaderFeature::FragmentOutline]             = ShaderStage::Fragment;    // IMPLEMENT FRAG OUTLINE
        // map[ShaderFeature::FragmentCubemap]             = ShaderStage::Fragment;    // IMPLEMENT FRAG CUBEMAP
        // map[ShaderFeature::FragmentBlending]            = ShaderStage::Fragment;    // IMPLEMENT FRAG BLENDING
        // map[ShaderFeature::FragmentShadows]             = ShaderStage::Fragment;    // IMPLEMENT FRAG SHADOWS

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
            _featureNames[ShaderInfo::ShaderFeature::VertexMVP]                    = "VertexMVP";
            _featureNames[ShaderInfo::ShaderFeature::VertexNormalsToColor]         = "VertexNormalsToColor";
            // _featureNames[ShaderInfo::ShaderFeature::VertexFishEye]                = "VertexFishEye";       // IMPLEMENT VERT LENS
            // _featureNames[ShaderInfo::ShaderFeature::GeometryShowNormals]          = "GeometryShowNormals"; // IMPLEMENT GEOM NORMALS
            _featureNames[ShaderInfo::ShaderFeature::FragmentFullColor]            = "FragmentFullColor";
            _featureNames[ShaderInfo::ShaderFeature::FragmentDepthView]            = "FragmentDepthView";
            _featureNames[ShaderInfo::ShaderFeature::FragmentMeshMaterial]         = "FragmentMeshMaterial";
            _featureNames[ShaderInfo::ShaderFeature::FragmentBypassVertexColor]    = "FragmentBypassVertexColor";
            // _featureNames[ShaderInfo::ShaderFeature::FragmentFlatShading]          = "FragmentFlatShading"; // IMPLEMENT FRAG FLAT
            _featureNames[ShaderInfo::ShaderFeature::FragmentPhong]                = "FragmentPhong";
            _featureNames[ShaderInfo::ShaderFeature::FragmentBlinnPhong]           = "FragmentBlinnPhong";
            _featureNames[ShaderInfo::ShaderFeature::FragmentGammaCorrection]      = "FragmentGammaCorrection";
            // _featureNames[ShaderInfo::ShaderFeature::FragmentOutline]              = "FragmentOutline";     // IMPLEMENT FRAG OUTLINE
            // _featureNames[ShaderInfo::ShaderFeature::FragmentCubemap]              = "FragmentCubemap";     // IMPLEMENT FRAG CUBEMAP
            // _featureNames[ShaderInfo::ShaderFeature::FragmentBlending]             = "FragmentBlending";    // IMPLEMENT FRAG BLENDING
            // _featureNames[ShaderInfo::ShaderFeature::FragmentShadows]              = "FragmentShadows";     // IMPLEMENT FRAG SHADOWS

            runOnce = true;
        }

        auto it = _featureNames.find(v);
        return (it != _featureNames.end()) ?
            it->second : "Unknown";
    }
}
