#include "shader_feature.hpp"

#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

namespace ShaderInfo
{
    const ShaderFeatureToShaderStageMap& FeatureStages()
    {
        static bool runOnce = false;
        static ShaderFeatureToShaderStageMap map;

        if (!runOnce)
        {
            map[ShaderFeature::VertexMVP]                       = ShaderStage::Vertex;
            map[ShaderFeature::VertexNormalsToColor]            = ShaderStage::Vertex;
            // map[ShaderFeature::VertexFishEye]                   = ShaderStage::Vertex;      // IMPLEMENT VERT LENS
            // map[ShaderFeature::GeometryShowNormals]             = ShaderStage::Geometry;    // IMPLEMENT GEOM NORMALS
            map[ShaderFeature::FragmentFullLight]               = ShaderStage::Fragment;
            map[ShaderFeature::FragmentViewDepthBuffer]         = ShaderStage::Fragment;
            map[ShaderFeature::FragmentViewLightAttenuation]    = ShaderStage::Fragment;
            map[ShaderFeature::FragmentMeshMaterial]            = ShaderStage::Fragment;
            map[ShaderFeature::FragmentBypassVertexColor]       = ShaderStage::Fragment;
            // map[ShaderFeature::FragmentFlatShading]             = ShaderStage::Fragment;    // IMPLEMENT FRAG FLAT
            map[ShaderFeature::FragmentPhong]                   = ShaderStage::Fragment;
            map[ShaderFeature::FragmentBlinnPhong]              = ShaderStage::Fragment;
            map[ShaderFeature::FragmentGammaCorrection]         = ShaderStage::Fragment;
            // map[ShaderFeature::FragmentOutline]                 = ShaderStage::Fragment;    // IMPLEMENT FRAG OUTLINE
            // map[ShaderFeature::FragmentCubemap]                 = ShaderStage::Fragment;    // IMPLEMENT FRAG CUBEMAP
            // map[ShaderFeature::FragmentBlending]                = ShaderStage::Fragment;    // IMPLEMENT FRAG BLENDING
            // map[ShaderFeature::FragmentShadows]                 = ShaderStage::Fragment;    // IMPLEMENT FRAG SHADOWS

            runOnce = true;
        }

        return map;
    }
}

namespace std
{
    string to_string(ShaderInfo::ShaderFeature v)
    {
        static bool runOnce = false;
        static unordered_map<ShaderInfo::ShaderFeature, string> featureNames;

        if (!runOnce)
        {
            featureNames[ShaderInfo::ShaderFeature::VertexMVP]                          = "VertexMVP";
            featureNames[ShaderInfo::ShaderFeature::VertexNormalsToColor]               = "VertexNormalsToColor";
            // featureNames[ShaderInfo::ShaderFeature::VertexFishEye]                      = "VertexFishEye";       // IMPLEMENT VERT LENS
            // featureNames[ShaderInfo::ShaderFeature::GeometryShowNormals]                = "GeometryShowNormals"; // IMPLEMENT GEOM NORMALS
            featureNames[ShaderInfo::ShaderFeature::FragmentFullLight]                  = "FragmentFullLight";
            featureNames[ShaderInfo::ShaderFeature::FragmentViewDepthBuffer]            = "FragmentViewDepthBuffer";
            featureNames[ShaderInfo::ShaderFeature::FragmentViewLightAttenuation]       = "FragmentViewLightAttenuation";
            featureNames[ShaderInfo::ShaderFeature::FragmentMeshMaterial]               = "FragmentMeshMaterial";
            featureNames[ShaderInfo::ShaderFeature::FragmentBypassVertexColor]          = "FragmentBypassVertexColor";
            // featureNames[ShaderInfo::ShaderFeature::FragmentFlatShading]                = "FragmentFlatShading"; // IMPLEMENT FRAG FLAT
            featureNames[ShaderInfo::ShaderFeature::FragmentPhong]                      = "FragmentPhong";
            featureNames[ShaderInfo::ShaderFeature::FragmentBlinnPhong]                 = "FragmentBlinnPhong";
            featureNames[ShaderInfo::ShaderFeature::FragmentGammaCorrection]            = "FragmentGammaCorrection";
            // featureNames[ShaderInfo::ShaderFeature::FragmentOutline]                    = "FragmentOutline";     // IMPLEMENT FRAG OUTLINE
            // featureNames[ShaderInfo::ShaderFeature::FragmentCubemap]                    = "FragmentCubemap";     // IMPLEMENT FRAG CUBEMAP
            // featureNames[ShaderInfo::ShaderFeature::FragmentBlending]                   = "FragmentBlending";    // IMPLEMENT FRAG BLENDING
            // featureNames[ShaderInfo::ShaderFeature::FragmentShadows]                    = "FragmentShadows";     // IMPLEMENT FRAG SHADOWS

            runOnce = true;
        }

        auto it = featureNames.find(v);
        return (it != featureNames.end()) ?
            it->second : "Unknown shader feature";
    }
}
