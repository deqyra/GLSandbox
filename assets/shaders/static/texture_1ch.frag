#version 420 core
#extension GL_ARB_shading_language_include : require

#include </interface_blocks/vertex_out>
in VertexOut vertOut;
out vec4 fragColor;

uniform sampler2D texData;

void main()
{
	fragColor = texture(texData, vertOut.texCoord) * vec4(vertOut.color, 1.0f); 
}