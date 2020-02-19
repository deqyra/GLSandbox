#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 2) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D texData1;
uniform sampler2D texData2;
uniform float textureMix;

void main()
{
	fragColor = mix(texture(texData1, inTexCoord), texture(texData2, inTexCoord), textureMix) * vec4(inVertexColor, 1.0f); 
}