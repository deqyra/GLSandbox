#ifndef TEMPLATES__PHONG
#define TEMPLATES__PHONG

vec3 phong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight)
{
	vec3 viewDirection = normalize(vertOut.fragPos);

	// Ambient lighting
    vec3 total = ambientLight;
#ifdef FRAGMENT_MESH_MATERIAL
	total *= material.ambient;
#endif//FRAGMENT_MESH_MATERIAL

	// Diffuse lighting
	float diffusionFactor = max(dot(vertOut.normal, -lightDirection), 0.0);
	vec3 diffuseTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.diffuseMapCount > 0)
	{
		vec4 sample = texture(material.diffuseMaps[0], vertOut.texCoord);
		diffuseTexel = sample.xyz * sample.w;
	}
	diffuseTexel *= material.diffuse;
#endif//FRAGMENT_MESH_MATERIAL
	
	total += (diffuseTexel * diffuseLight) * diffusionFactor;

	// Specular
	vec3 specularTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.specularMapCount > 0)
	{
		vec4 sample = texture(material.specularMaps[0], vertOut.texCoord);
		specularTexel = sample.xyz * sample.w;
	}
#endif//FRAGMENT_MESH_MATERIAL

#ifdef FRAGMENT_BLINN_PHONG
	vec3 halfwayDirection = normalize(viewDirection + lightDirection);
	float spec = pow(max(dot(vertOut.normal, -halfwayDirection), 0.0), material.shininess);
#else
	vec3 reflectDirection = reflect(lightDirection, vertOut.normal);
	float spec = pow(max(dot(-viewDirection, reflectDirection), 0.0), material.shininess);
#endif//FRAGMENT_BLINN_PHONG 

	total += specularLight * material.specular * specularTexel * spec;

	return total;
}

#endif//TEMPLATES__PHONG