#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D uTexData;

void main()
{
	fragColor = texture(uTexData, inTexCoord) * vec4(inVertexColor, 1.0f); 
}