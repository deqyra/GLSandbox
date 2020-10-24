#version 420 core
#extension GL_ARB_shading_language_include : require

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

#include </interface_blocks/vertex_out>
out VertexOut vertOut;

#include </uniform_blocks/matrices>

void main()
{
    gl_Position = matrices.projection * matrices.view * matrices.model * vec4(inPosition, 1.0f);
	vertOut.color = inColor;
	vertOut.normal = matrices.normal * inNormal;
	vertOut.texCoord = inTexCoord;
	vertOut.fragPos = vec3(matrices.view * matrices.model * vec4(inPosition, 1.0f));
}