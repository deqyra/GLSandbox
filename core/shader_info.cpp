#include "shader_info.hpp"

const ShaderInfo::ShaderToIncludeDirectiveMap ShaderInfo::IncludeDirectives = ShaderInfo::getIncludeRequirements();
bool ShaderInfo::_includeDirectivessPopulated = false;

const ShaderInfo::IncludeToFilenameMap ShaderInfo::IncludeFilenames = ShaderInfo::getIncludeFilenames();
bool ShaderInfo::_includeFilenamesPopulated = false;

namespace ShaderInfo
{
    ShaderToIncludeDirectiveMap getIncludeRequirements()
    {
        if (_includeDirectivessPopulated) return IncludeDirectives;

        ShaderToIncludeDirectiveMap map;

        map["assets/shaders/mvp.vert"]      = {"/uniform_blocks/matrices"};
        map["assets/shaders/phong.frag"]    = {"/uniform_blocks/lights",
                                               "/uniform_blocks/material"};

        return map;
    }

    IncludeToFilenameMap getIncludeFilenames()
    {
        if (_includeFilenamesPopulated) return IncludeFilenames;

        IncludeToFilenameMap map;

        map["/uniform_blocks/matrices"]     = "assets/shaders/uniform_blocks/matrices.glsl";
        map["/uniform_blocks/lights"]       = "assets/shaders/uniform_blocks/lights.glsl";
        map["/uniform_blocks/material"]     = "assets/shaders/uniform_blocks/material.glsl";

        return map;
    }
}