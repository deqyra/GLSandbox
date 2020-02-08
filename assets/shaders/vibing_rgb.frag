#version 460 core
layout (location = 0) in vec3 inVertexColor;

out vec4 fragColor;

uniform float uTime;
#define M_PI 3.1415926535897932384626433832795

float sine_offset(float x, float t)
{
	float asin_res = asin(2 * x - 1);
	return (sin(asin_res + (t * M_PI)) + 1) / 2;
}

void main()
{
    fragColor  = vec4(sine_offset(inVertexColor.x, uTime),
					  sine_offset(inVertexColor.y, uTime + 0.333333333),
					  sine_offset(inVertexColor.z, uTime + 0.666666666),
					  1.0f);
}
