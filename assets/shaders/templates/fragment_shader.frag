#include </interface_blocks/vertex_out>
in VertexOut vertOut;

out vec4 fragColor;

#ifdef FRAGMENT_VIEW_DEPTH_BUFFER
	uniform float near = 0.1f; 
	uniform float far  = 100.f; 
	
	float linearizeDepth(float depth) 
	{
		float z = depth * 2.f - 1.f;
		return (2.f * near * far) / (far + near - z * (far - near));	
	}
#endif//FRAGMENT_VIEW_DEPTH_BUFFER

#ifdef FRAGMENT_MESH_MATERIAL
	#include </uniform_blocks/material>
#endif//FRAGMENT_MESH_MATERIAL

#ifdef FRAGMENT_BLINN_PHONG
	// Blinn-Phong requires Phong, the Blinn part is extra
	#define FRAGMENT_PHONG
#endif//FRAGMENT_BLINN_PHONG

#ifdef FRAGMENT_PHONG || FRAGMENT_VIEW_LIGHT_ATTENUATION
	#define REQUIRE_LIGHTS
#endif//FRAGMENT_PHONG || FRAGMENT_VIEW_LIGHT_ATTENUATION

#ifdef REQUIRE_LIGHTS
	#include </uniform_blocks/lights>
	#include </functional_blocks/light_attenuation>
#endif//REQUIRE_LIGHTS

#ifdef FRAGMENT_PHONG
	#include </templates/phong>
#endif//FRAGMENT_PHONG

#ifdef FRAGMENT_GAMMA_CORRECTION
	uniform float gamma = 2.2f;
	#include </functional_blocks/gamma_correction>
#endif//FRAGMENT_GAMMA_CORRECTION

void main()
{
	vec4 color = vec4(0.f, 0.f, 0.f, 0.f);
#ifdef FRAGMENT_FULL_LIGHT
	color = vec4(1.f);
	#ifdef FRAGMENT_MESH_MATERIAL
		if (material.diffuseMapCount > 0)
			color = vec3(texture(material.diffuseMaps[0], vertOut.texCoord));
	#endif//FRAGMENT_MESH_MATERIAL
#endif//FRAGMENT_FULL_LIGHT

#ifdef FRAGMENT_VIEW_DEPTH_BUFFER
    color = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.f);
#endif//FRAGMENT_VIEW_DEPTH_BUFFER

#ifdef FRAGMENT_PHONG
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
		float intensity = clamp((theta - outerCos) / epsilon, 0.f, 1.f);

		float dist = length(positionDiff);
		colorTotal += intensity
					* attenuate(lights.spot[i].constant,
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

    color = vec4(colorTotal, 1.f);
#endif//FRAGMENT_PHONG

#ifdef FRAGMENT_VIEW_LIGHT_ATTENUATION
	vec3 colorTotal = vec3(0.f);
	for (int i = 0; i < lights.pointCount; i++)
	{
		vec3 positionDiff = vertOut.fragPos - lights.point[i].position;
		float dist = length(positionDiff);
		colorTotal += vec3(attenuate(lights.point[i].constant,
									 lights.point[i].linear,
									 lights.point[i].quadratic,
									 dist));
	}
	for (int i = 0; i < lights.spotCount; i++)
	{
		vec3 positionDiff = vertOut.fragPos - lights.spot[i].position;
		vec3 direction = normalize(positionDiff);

		float theta = dot(direction, normalize(lights.spot[i].direction));
		float outerCos = cos(lights.spot[i].outerCutoff);
		float innerCos = cos(lights.spot[i].innerCutoff);
		float epsilon = innerCos - outerCos;
		float intensity = clamp((theta - outerCos) / epsilon, 0.f, 1.f);

		float dist = length(positionDiff);
		colorTotal += intensity * vec3(attenuate(lights.spot[i].constant,
												 lights.spot[i].linear,
												 lights.spot[i].quadratic,
												 dist));
	}

    color = vec4(colorTotal, 1.f);
#endif//FRAGMENT_VIEW_LIGHT_ATTENUATION

#ifndef FRAGMENT_BYPASS_VERTEX_COLOR
	color *= vec4(vertOut.color, 1.f);
#endif//FRAGMENT_BYPASS_VERTEX_COLOR

#ifdef FRAGMENT_GAMMA_CORRECTION
	fragColor = gammaCorrect(color, gamma);
#else
	fragColor = color;
#endif//FRAGMENT_GAMMA_CORRECT

	vec3 color2 = vec3(0.f);
	color2 = vec3(attenuate(lights.point[0].constant,
						lights.point[0].linear,
						lights.point[0].quadratic,
						length(vertOut.fragPos - lights.point[0].position)));
	fragColor = vec4(color2, 1.f);
}
