#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec2 inTexCoord;

out vec4 fragColor;

void main()
{
	fragColor = vec4(inVertexColor, 1.0f); 
}