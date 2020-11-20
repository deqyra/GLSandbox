#ifndef CORE__SHADER__SHADER_FEATURE_HPP
#define CORE__SHADER__SHADER_FEATURE_HPP

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

    /// @brief Structure mapping shader features to the shader stage which they
    /// are implemented in.
    extern const ShaderFeatureToShaderStageMap FeatureStages;

    /// @brief Get the map describing in which stage shader features are
    /// implemented.
    ///
    /// @return A map describing in which stage shader features are implemented.
    const ShaderFeatureToShaderStageMap& getFeatureStages();
}

namespace std
{
    std::string to_string(ShaderInfo::ShaderFeature v);
}

#endif//CORE__SHADER__SHADER_FEATURE_HPP