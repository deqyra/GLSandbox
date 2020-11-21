#ifndef CORE__SHADER__SHADER_FEATURE_HPP
#define CORE__SHADER__SHADER_FEATURE_HPP

#include <string>
#include <unordered_map>

#include "shader_stage.hpp"

namespace ShaderInfo
{
    /// @brief Collection of literals describing the different aspects which
    /// a shader might support when rendering a scene or its elements.
    enum class ShaderFeature
    {
        VertexMVP,
        // VertexFishEye,          // IMPLEMENT VERT LENS
        VertexNormalsToColor,
        // GeometryShowNormals,    // IMPLEMENT GEOM NORMALS
        FragmentFullColor,
        FragmentDepthView,
        FragmentMeshMaterial,
        FragmentBypassVertexColor,
        // FragmentFlatShading,    // IMPLEMENT FRAG FLAT
        FragmentPhong,
        FragmentBlinnPhong,
        FragmentGammaCorrection
        // FragmentOutline,        // IMPLEMENT FRAG OUTLINE
        // FragmentCubemap,        // IMPLEMENT FRAG CUBEMAP
        // FragmentBlending,       // IMPLEMENT FRAG BLENDING
        // FragmentShadows         // IMPLEMENT FRAG SHADOWS
    };

    using ShaderFeatureToShaderStageMap = std::unordered_map<ShaderFeature, ShaderStage>;

    /// @brief Get the map describing in which stage shader features are
    /// implemented.
    ///
    /// @return A map describing in which stage shader features are implemented.
    const ShaderFeatureToShaderStageMap& FeatureStages();
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderFeature v);
}

#endif//CORE__SHADER__SHADER_FEATURE_HPP