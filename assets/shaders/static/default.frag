#version 420 core

#include </interface_blocks/vertex_out>
in VertexOut vertOut;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vertOut.color, 1.0f); 
}