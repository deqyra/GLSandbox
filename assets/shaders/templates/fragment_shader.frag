#include </interface_blocks/vertex_out>
in VertexOut vertOut;

out vec4 fragColor;

#ifdef FRAGMENT_DEPTH_VIEW
	uniform float near = 0.1; 
	uniform float far  = 100.0; 
	
	float linearizeDepth(float depth) 
	{
		float z = depth * 2.0 - 1.0;
		return (2.0 * near * far) / (far + near - z * (far - near));	
	}
#endif//FRAGMENT_DEPTH_VIEW

#ifdef FRAGMENT_MESH_MATERIAL
	#include </uniform_blocks/material>
#endif//FRAGMENT_MESH_MATERIAL

#ifdef FRAGMENT_BLINN_PHONG
	// Blinn-Phong requires Phong, the Blinn part is extra
	#define FRAGMENT_PHONG
#endif//FRAGMENT_BLINN_PHONG

#ifdef FRAGMENT_PHONG
	#include </uniform_blocks/lights>
	#include </functional_blocks/light_attenuation>

	#include </templates/phong>
#endif//FRAGMENT_PHONG

#ifdef FRAGMENT_GAMMA_CORRECTION
	uniform float gamma = 2.2;
	#include </functional_blocks/gamma_correction>
#endif//FRAGMENT_GAMMA_CORRECTION

void main()
{
	vec4 color;
#ifdef FRAGMENT_FULL_COLOR
	color = vec3(vertOut.color, 1.0f);
#endif//FRAGMENT_FULL_COLOR

#ifdef FRAGMENT_DEPTH_VIEW
    color = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.0);
#endif//FRAGMENT_DEPTH_VIEW

#ifdef FRAGMENT_PHONG
	// Process all point lights
	vec3 colorTotal = vec3(0.f);
	for (int i = 0; i < lights.pointCount; i++)
	{
		vec3 positionDiff = vertOut.fragPos - lights.point[i].position;
		vec3 direction = normalize(positionDiff);
		float dist = length(positionDiff);
		colorTotal += attenuate(lights.point[i].constant,
								lights.point[i].linear,
								lights.point[i].quadratic,
								dist)
					* phong(direction,
							lights.point[i].ambient,
							lights.point[i].diffuse,
							lights.point[i].specular);
	}
	for (int i = 0; i < lights.spotCount; i++)
	{
		vec3 positionDiff = vertOut.fragPos - lights.spot[i].position;
		vec3 direction = normalize(positionDiff);

		float theta = dot(direction, normalize(lights.spot[i].direction));
		float outerCos = cos(lights.spot[i].outerCutoff);
		float innerCos = cos(lights.spot[i].innerCutoff);
		float epsilon = innerCos - outerCos;
		float intensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);

		float dist = length(positionDiff);
		colorTotal += attenuate(lights.spot[i].constant,
								lights.spot[i].linear,
								lights.spot[i].quadratic,
								dist)
					* phong(direction,
							lights.spot[i].ambient,
							lights.spot[i].diffuse,
							lights.spot[i].specular);
	}
	for (int i = 0; i < lights.directionalCount; i++)
	{
		colorTotal += phong(lights.direct[i].direction,
							lights.direct[i].ambient,
							lights.direct[i].diffuse,
							lights.direct[i].specular);
	}

    vec4 color = vec4(colorTotal, 1.f);
	#ifndef FRAGMENT_BYPASS_VERTEX_COLOR
		color *= vec4(vertOut.color, 1.f);
	#endif//FRAGMENT_BYPASS_VERTEX_COLOR
#endif//FRAGMENT_PHONG

#ifdef FRAGMENT_GAMMA_CORRECTION
	fragColor = gammaCorrect(color, gamma);
#else
	fragColor = color;
#endif//FRAGMENT_GAMMA_CORRECTION
}
