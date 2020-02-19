#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec3 inVertexNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inFragPos;

out vec4 fragColor;

// Types and constants
// ===================

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

const int N_POINT_LIGHTS = 256;

// Uniforms
// ========

uniform PointLight pLights[N_POINT_LIGHTS];
uniform int nLights = 0;

uniform Material material;


// Functions
// =========

vec3 processPointLight(int i);

void main()
{
	vec3 pLightTotal = vec3(0.f);
	for (int i = 0; i < nLights; i++)
	{
		pLightTotal += processPointLight(i);
	}

    vec3 result = pLightTotal * inVertexColor;
    fragColor = vec4(result, 1.0);
}

vec3 processPointLight(int i)
{
	vec3 normal = normalize(inVertexNormal);
	vec3 viewDir = normalize(inFragPos);
	vec3 lightDir = normalize(inFragPos - pLights[i].position);

	// Ambient lighting
    vec3 ambient = pLights[i].ambient * material.ambient;

	// Diffuse lighting
	float diffusionFactor = max(dot(normal, -lightDir), 0.0);
	vec3 diffuse = pLights[i].diffuse * material.diffuse * diffusionFactor;

	// Specular
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pLights[i].specular * material.specular * spec;

	return ambient + diffuse + specular;
}