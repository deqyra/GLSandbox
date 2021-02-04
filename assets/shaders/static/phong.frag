#version 420 core

#include </interface_blocks/vertex_out>
in VertexOut vertOut;

out vec4 fragColor;

#include </uniform_blocks/material>
#include </uniform_blocks/lights>

uniform float gamma = 2.2;

#include </functional_blocks/gamma_correction>

// Functions
// =========

vec3 processPointLight(int i);
vec3 processSpotLight(int i);
vec3 processDirectionalLight(int i);
vec3 calculatePhong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight);

void main()
{
	// Process all point lights
	vec3 lightTotal = vec3(0.f);
	for (int i = 0; i < lights.pointCount; i++)
	{
		lightTotal += processPointLight(i);
	}
	for (int i = 0; i < lights.spotCount; i++)
	{
		lightTotal += processSpotLight(i);
	}
	for (int i = 0; i < lights.directionalCount; i++)
	{
		lightTotal += processDirectionalLight(i);
	}	

	// Combine components together
    vec4 color = vec4(lightTotal, 1.f) * vec4(vertOut.color, 1.f);
	fragColor = gammaCorrect(color, gamma);
	// fragColor = color;
}

vec3 processPointLight(int i)
{
	vec3 positionDiff = vertOut.fragPos - lights.point[i].position;
	vec3 lightDirection = normalize(positionDiff);
	float dist = length(positionDiff);
	float attenuation = 1.f / (	 lights.point[i].constant +
								(lights.point[i].linear * dist) +
								(lights.point[i].quadratic * dist * dist)
							  );

	return attenuation * calculatePhong(lightDirection,
										lights.point[i].ambient,
										lights.point[i].diffuse,
										lights.point[i].specular);
}

vec3 processSpotLight(int i)
{
	vec3 positionDiff = vertOut.fragPos - lights.spot[i].position;
	vec3 lightDirection = normalize(positionDiff);

	float theta = dot(lightDirection, normalize(lights.spot[i].direction));
	float outerCos = cos(lights.spot[i].outerCutoff);
	float innerCos = cos(lights.spot[i].innerCutoff);
	float epsilon = innerCos - outerCos;
	float intensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);

	float dist = length(positionDiff);
	float attenuation = 1.0 / (	 lights.spot[i].constant + 
								(lights.spot[i].linear * dist) + 
								(lights.spot[i].quadratic * dist * dist)
							  );

	return attenuation * intensity * calculatePhong(lightDirection,
													lights.spot[i].ambient,
													lights.spot[i].diffuse,
													lights.spot[i].specular);
}

vec3 processDirectionalLight(int i)
{
	return calculatePhong(lights.direct[i].direction,
						  lights.direct[i].ambient,
						  lights.direct[i].diffuse,
						  lights.direct[i].specular);
}

vec3 calculatePhong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight)
{
	vec3 normal = vertOut.normal;
	vec3 viewDir = normalize(vertOut.fragPos);
	vec3 reflectDir = reflect(lightDirection, normal);
	vec3 halfwayDir = normalize(viewDir + lightDirection);

	// Ambient lighting
    vec3 ambient = ambientLight * material.ambient;

	// Diffuse lighting
	vec4 diffuseTexel = vec4(1.f);
	if (material.diffuseMapCount > 0)
		diffuseTexel = texture(material.diffuseMaps[0], vertOut.texCoord);
	vec3 diffuseTexel3 = vec3(diffuseTexel * diffuseTexel.a);
	
	float diffusionFactor = max(dot(normal, -lightDirection), 0.0);
	vec3 diffuse = diffuseLight * material.diffuse * diffuseTexel3 * diffusionFactor;

	// Specular
	vec4 specularTexel = vec4(1.f);
	if (material.specularMapCount > 0)
		specularTexel = texture(material.specularMaps[0], vertOut.texCoord);
	vec3 specularTexel3 = vec3(specularTexel * specularTexel.a);

	// float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	float spec = pow(max(dot(normal, -halfwayDir), 0.0), material.shininess);
	vec3 specular = specularLight * material.specular * specularTexel3 * spec;

	return ambient + diffuse + specular;
}
