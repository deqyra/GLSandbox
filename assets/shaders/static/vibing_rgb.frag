#version 420 core
#extension GL_ARB_shading_language_include : require

#include </interface_blocks/vertex_out>
in VertexOut vertOut;
out vec4 fragColor;

uniform float time;
#define M_PI 3.1415926535897932384626433832795

float sine_offset(float x, float t)
{
	float asin_res = asin(2 * x - 1);
	return (sin(asin_res + (t * M_PI)) + 1) / 2;
}

void main()
{
    fragColor  = vec4(sine_offset(vertOut.color.x, time),
					  sine_offset(vertOut.color.y, time + 0.333333333),
					  sine_offset(vertOut.color.z, time + 0.666666666),
					  1.0f);
}
