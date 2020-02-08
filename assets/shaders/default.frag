#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D uTexData1;
uniform sampler2D uTexData2;
uniform float uTextureMix;

void main()
{
	fragColor = mix(texture(uTexData1, inTexCoord), texture(uTexData2, inTexCoord), uTextureMix) * vec4(inVertexColor, 1.0f); 
}