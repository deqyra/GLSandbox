#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec3 inVertexNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inFragPos;

struct Light {
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
  
uniform Light uLight;
uniform Material uMaterial;

out vec4 fragColor;

void main()
{
	vec3 norm = normalize(inVertexNormal);
	vec3 lightDir = normalize(inFragPos - uLight.position);

	// Ambient lighting
    vec3 ambient = uLight.ambient * uMaterial.ambient;

	// Diffuse lighting
	float diffusionFactor = max(dot(norm, -lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * uMaterial.diffuse * diffusionFactor;

	// Specular lighting
	vec3 viewDir = normalize(inFragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * uMaterial.specular * spec;

    vec3 result = (ambient + diffuse + specular) * inVertexColor;
    fragColor = vec4(result, 1.0);
}  