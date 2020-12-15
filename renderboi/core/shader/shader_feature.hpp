#ifndef RENDERBOI__CORE__SHADER__SHADER_FEATURE_HPP
#define RENDERBOI__CORE__SHADER__SHADER_FEATURE_HPP

#include <string>
#include <unordered_map>

#include "shader_stage.hpp"

namespace ShaderInfo
{
    /// @brief Collection of literals describing the different aspects which
    /// a shader might support when rendering a scene or its elements.
    enum class ShaderFeature
    {
        /// @brief The vertex stage of the shader will handle MVP matrices.
        /// Requires: nothing.
        /// Incompatible with: nothing.
        VertexMVP,

        // VertexFishEye,          // IMPLEMENT VERT LENS

        /// @brief The vertex stage of the shader will output vertex normals as
        /// vertex colors.
        /// Requires: VertexMVP.
        /// Incompatible with: nothing.
        VertexNormalsToColor,

        // GeometryShowNormals,    // IMPLEMENT GEOM NORMALS

        /// @brief The fragment stage of the shader will render objects with
        /// full light, ignoring actual lighting.
        /// Requires: nothing.
        /// Incompatible with: FragmentDepthView, FragmentPhong,
        /// FragmentBlinnPhong.
        FragmentFullLight,

        /// @brief The fragment stage of the shader will render objects as they
        /// appear in the depth buffer, ignoring lighting.
        /// Requires: nothing.
        /// Incompatible with: FragmentFullLight, FragmentPhong,
        /// FragmentBlinnPhong.
        FragmentDepthView,

        /// @brief The fragment stage of the shader will handle materials to 
        /// render objects with.
        /// Requires: nothing.
        /// Incompatible with: nothing.
        FragmentMeshMaterial,

        /// @brief The fragment stage of the shader will ignore vertex color
        /// entirely.
        /// Requires: nothing.
        /// Incompatible with: nothing.
        FragmentBypassVertexColor,

        // FragmentFlatShading,    // IMPLEMENT FRAG FLAT
        
        /// @brief The fragment stage of the shader will calculate object
        /// illumination using a Phong lighting model.
        /// Requires: FragmentMeshMaterial.
        /// Incompatible with: FragmentFullLight, FragmentDepthView,
        /// FragmentBlinnPhong.
        FragmentPhong,
        
        /// @brief The fragment stage of the shader will calculate object
        /// illumination using a Blinn-Phong lighting model.
        /// Requires: FragmentMeshMaterial.
        /// Incompatible with: FragmentFullLight, FragmentDepthView,
        /// FragmentPhong.
        FragmentBlinnPhong,
        
        /// @brief The fragment stage of the shader will correct the output 
        /// color according to a gamma value.
        /// Requires: nothing.
        /// Incompatible with: nothing.
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

#endif//RENDERBOI__CORE__SHADER__SHADER_FEATURE_HPP