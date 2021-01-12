#version 420 core

#include </interface_blocks/vertex_attributes>

#include </interface_blocks/vertex_out>
out VertexOut vertOut;

#include </uniform_blocks/matrices>

void main()
{
    gl_Position = matrices.projection * matrices.view * matrices.model * vec4(inPosition, 1.0f);
	vertOut.color = inColor;
	vertOut.normal = normalize(matrices.normal * inNormal);
	vertOut.texCoord = inTexCoord;
	vertOut.fragPos = vec3(matrices.view * matrices.model * vec4(inPosition, 1.0f));
}