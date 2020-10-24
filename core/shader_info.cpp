#include "shader_info.hpp"

const ShaderInfo::ShaderToIncludeDirectiveMap ShaderInfo::IncludeDirectives = ShaderInfo::getIncludeRequirements();
bool ShaderInfo::_includeDirectivesPopulated = false;

const ShaderInfo::IncludeToFilenameMap ShaderInfo::IncludeFilenames = ShaderInfo::getIncludeFilenames();
bool ShaderInfo::_includeFilenamesPopulated = false;

const ShaderInfo::ShaderToSupportedFeatureMap ShaderInfo::SupportedFeatures = ShaderInfo::getSupportedFeatures();
bool ShaderInfo::_supportedFeaturesPopulated = false;

namespace ShaderInfo
{
    ShaderToIncludeDirectiveMap getIncludeRequirements()
    {
        if (_includeDirectivesPopulated) return IncludeDirectives;

        ShaderToIncludeDirectiveMap map;

        map["assets/shaders/mvp.vert"]      = {"/uniform_blocks/matrices",
                                               "/interface_blocks/vertex_out"};
        map["assets/shaders/phong.frag"]    = {"/uniform_blocks/lights",
                                               "/uniform_blocks/material",
                                               "/interface_blocks/vertex_out"};
        map["assets/shaders/default.frag"]  = {"/interface_blocks/vertex_out"};
        map["assets/shaders/texture_1ch.frag"]  = {"/interface_blocks/vertex_out"};
        map["assets/shaders/texture_2ch.frag"]  = {"/interface_blocks/vertex_out"};
        map["assets/shaders/vibing_rgb.frag"]  = {"/interface_blocks/vertex_out"};

        return map;
    }

    IncludeToFilenameMap getIncludeFilenames()
    {
        if (_includeFilenamesPopulated) return IncludeFilenames;

        IncludeToFilenameMap map;

        map["/uniform_blocks/matrices"]     = "assets/shaders/uniform_blocks/matrices.glsl";
        map["/uniform_blocks/lights"]       = "assets/shaders/uniform_blocks/lights.glsl";
        map["/uniform_blocks/material"]     = "assets/shaders/uniform_blocks/material.glsl";
        map["/interface_blocks/vertex_out"] = "assets/shaders/interface_blocks/vertex_out.glsl";

        return map;
    }

    ShaderToSupportedFeatureMap getSupportedFeatures()
    {
        if (_supportedFeaturesPopulated) return SupportedFeatures;

        ShaderToSupportedFeatureMap map;

        map["assets/shaders/phong.frag"]    = {RenderFeature::Material};

        return map;
    }
}