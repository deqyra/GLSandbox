#version 420 core
in VertexOut 
{
	vec3 fragPos;
	vec3 color;
	vec3 normal;
	vec2 texCoord;
} vertOut;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vertOut.color, 1.0f); 
}