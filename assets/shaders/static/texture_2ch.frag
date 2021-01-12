#version 420 core

#include </interface_blocks/vertex_out>
in VertexOut vertOut;
out vec4 fragColor;

uniform sampler2D texData1;
uniform sampler2D texData2;
uniform float textureMix;

void main()
{
	fragColor = mix(texture(texData1, vertOut.texCoord), texture(texData2, vertOut.texCoord), textureMix) * vec4(vertOut.color, 1.0f); 
}