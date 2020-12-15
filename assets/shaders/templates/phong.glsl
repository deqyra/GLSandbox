#ifndef TEMPLATES__PHONG
#define TEMPLATES__PHONG

vec3 phong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight)
{
	vec3 normal = vertOut.normal;
	vec3 viewDirection = normalize(vertOut.fragPos);

	// Ambient lighting
    vec3 total = ambientLight;
#ifdef FRAGMENT_MESH_MATERIAL
	total *= material.ambient;
#endif//FRAGMENT_MESH_MATERIAL

	// Diffuse lighting
	float diffusionFactor = max(dot(normal, -lightDirection), 0.0);
	vec3 diffuseTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.diffuseMapCount > 0)
		diffuseTexel = vec3(texture(material.diffuseMaps[0], vertOut.texCoord));
	diffuseTexel *= material.diffuse;
#endif//FRAGMENT_MESH_MATERIAL
	
	total += (diffuseTexel * diffuseLight) * diffusionFactor;

	// Specular
	vec3 specularTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.specularMapCount > 0)
		specularTexel = vec3(texture(material.specularMaps[0], vertOut.texCoord));
#endif//FRAGMENT_MESH_MATERIAL

#if   defined FRAGMENT_BLINN_PHONG
	vec3 halfwayDirection = normalize(viewDirection + lightDirection);
	float spec = pow(max(dot(normal, -halfwayDirection), 0.0), material.shininess);
#elif defined FRAGMENT_PHONG
	vec3 reflectDirection = reflect(lightDirection, normal);
	float spec = pow(max(dot(-viewDirection, reflectDirection), 0.0), material.shininess);
#endif//FRAGMENT_BLINN_PHONG || FRAGMENT_PHONG

	total += specularLight * material.specular * specularTexel * spec;

	return total;
}

#endif//TEMPLATES__PHONG