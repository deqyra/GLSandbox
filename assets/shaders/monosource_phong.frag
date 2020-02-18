#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec3 inVertexNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inFragPos;

uniform vec3 uLightPos;
uniform vec3 uLightColor = vec3(1.f);
uniform float uAmbientStrength = 0.2f;
uniform float uSpecularStrength = 0.5f;
uniform uint uSpecularShininess = 256;

out vec4 fragColor;

void main()
{
	vec3 norm = normalize(inVertexNormal);
	vec3 lightDir = normalize(inFragPos - uLightPos);

	// Ambient lighting calculation
    vec3 ambient = uAmbientStrength * uLightColor;

	// Diffuse lighting calculation
	float diffusionFactor = max(dot(norm, -lightDir), 0.0);
	vec3 diffuse = diffusionFactor * uLightColor;

	// Specular lighting calculation
	vec3 viewDir = normalize(inFragPos);
	vec3 reflectDir = reflect(lightDir, norm);  
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), uSpecularShininess);
	vec3 specular = uSpecularStrength * spec * uLightColor;  

    vec3 result = (ambient + diffuse + specular) * inVertexColor;
    fragColor = vec4(result, 1.0);
}  