#include "shader_info.hpp"

namespace ShaderInfo
{
    const IncludeDirectiveToFilenameMap& IncludeFilenames()
    {
        static bool runOnce = false;
        static IncludeDirectiveToFilenameMap map;

        if (!runOnce)
        {
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
        }

        return map;
    }

    const FilenameToSupportedFeatureMap& FeaturesSupportedByFile()
    {
        static bool runOnce = false;
        static FilenameToSupportedFeatureMap map;

        if (!runOnce)
        {
            map["assets/shaders/default.frag"]  = {
                ShaderFeature::FragmentFullLight
            };
            map["assets/shaders/depth.frag"]  = {
                ShaderFeature::FragmentViewDepthBuffer
            };
            map["assets/shaders/mvp.vert"]  = {
                ShaderFeature::VertexMVP
            };
            map["assets/shaders/phong.frag"]    = {
                ShaderFeature::FragmentMeshMaterial,
                ShaderFeature::FragmentBlinnPhong,
                ShaderFeature::FragmentGammaCorrection
            };

            runOnce = true;
        }

        return map;
    }
}
