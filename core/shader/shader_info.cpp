#include "shader_info.hpp"

const ShaderInfo::IncludeDirectiveToFilenameMap ShaderInfo::IncludeFilenames = ShaderInfo::getIncludeFilenames();
const ShaderInfo::FilenameToSupportedFeatureMap ShaderInfo::SupportedFeatures = ShaderInfo::getSupportedFeatures();

namespace ShaderInfo
{
    const IncludeDirectiveToFilenameMap getIncludeFilenames()
    {
        static bool runOnce = false;
        if (runOnce) return IncludeFilenames;

        IncludeDirectiveToFilenameMap map;

        map["/functional_blocks/gamma_correction"]  = "assets/shaders/functional_blocks/gamma_correction.glsl";
        map["/functional_blocks/light_attenuation"] = "assets/shaders/functional_blocks/light_attenuation.glsl";
        map["/interface_blocks/light_types"]        = "assets/shaders/interface_blocks/light_types.glsl";
        map["/interface_blocks/vertex_attributes"]  = "assets/shaders/interface_blocks/vertex_attributes.glsl";
        map["/interface_blocks/vertex_out"]         = "assets/shaders/interface_blocks/vertex_out.glsl";
        map["/templates/phong"]                     = "assets/shaders/templates/phong.glsl";
        map["/uniform_blocks/lights"]               = "assets/shaders/uniform_blocks/lights.glsl";
        map["/uniform_blocks/material"]             = "assets/shaders/uniform_blocks/material.glsl";
        map["/uniform_blocks/matrices"]             = "assets/shaders/uniform_blocks/matrices.glsl";

        runOnce = true;
        return map;
    }

    const FilenameToSupportedFeatureMap getSupportedFeatures()
    {
        static bool runOnce = false;
        if (runOnce) return SupportedFeatures;

        FilenameToSupportedFeatureMap map;

        map["assets/shaders/default.frag"]  = {
            ShaderFeature::FragmentFullColor
        };
        map["assets/shaders/depth.frag"]  = {
            ShaderFeature::FragmentDepthView
        };
        map["assets/shaders/mvp.vert"]  = {
            ShaderFeature::VertexMVP
        };
        map["assets/shaders/phong.frag"]    = {
            ShaderFeature::FragmentMeshMaterial,
            ShaderFeature::FragmentBlinnPhong,
            ShaderFeature::FragmentMeshMaterial,
            ShaderFeature::FragmentGammaCorrection
        };

        runOnce = true;
        return map;
    }
}
