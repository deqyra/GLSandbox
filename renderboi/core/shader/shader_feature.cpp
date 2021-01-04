#include "shader_feature.hpp"

#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

namespace Renderboi
{

const std::unordered_map<ShaderFeature, ShaderStage>& FeatureStages()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderFeature, ShaderStage> map;

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

std::string to_string(const ShaderFeature v)
{
    static bool runOnce = false;
    static std::unordered_map<ShaderFeature, std::string> featureNames;

    if (!runOnce)
    {
        featureNames[ShaderFeature::VertexMVP]                      = "VertexMVP";
        featureNames[ShaderFeature::VertexNormalsToColor]           = "VertexNormalsToColor";
        // featureNames[ShaderFeature::VertexFishEye]                  = "VertexFishEye";       // IMPLEMENT VERT LENS
        // featureNames[ShaderFeature::GeometryShowNormals]            = "GeometryShowNormals"; // IMPLEMENT GEOM NORMALS
        featureNames[ShaderFeature::FragmentFullLight]              = "FragmentFullLight";
        featureNames[ShaderFeature::FragmentViewDepthBuffer]        = "FragmentViewDepthBuffer";
        featureNames[ShaderFeature::FragmentViewLightAttenuation]   = "FragmentViewLightAttenuation";
        featureNames[ShaderFeature::FragmentMeshMaterial]           = "FragmentMeshMaterial";
        featureNames[ShaderFeature::FragmentBypassVertexColor]      = "FragmentBypassVertexColor";
        // featureNames[ShaderFeature::FragmentFlatShading]            = "FragmentFlatShading"; // IMPLEMENT FRAG FLAT
        featureNames[ShaderFeature::FragmentPhong]                  = "FragmentPhong";
        featureNames[ShaderFeature::FragmentBlinnPhong]             = "FragmentBlinnPhong";
        featureNames[ShaderFeature::FragmentGammaCorrection]        = "FragmentGammaCorrection";
        // featureNames[ShaderFeature::FragmentOutline]                = "FragmentOutline";     // IMPLEMENT FRAG OUTLINE
        // featureNames[ShaderFeature::FragmentCubemap]                = "FragmentCubemap";     // IMPLEMENT FRAG CUBEMAP
        // featureNames[ShaderFeature::FragmentBlending]               = "FragmentBlending";    // IMPLEMENT FRAG BLENDING
        // featureNames[ShaderFeature::FragmentShadows]                = "FragmentShadows";     // IMPLEMENT FRAG SHADOWS

        runOnce = true;
    }

    auto it = featureNames.find(v);
    return (it != featureNames.end()) ?
        it->second : "Unknown shader feature";
}

}//namespace Renderboi
