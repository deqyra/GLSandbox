#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec3 inVertexNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inFragPos;

uniform vec3 uLightPos;
uniform vec3 uLightColor = vec3(1.f);
uniform float uAmbientStrength = 0.2f;

out vec4 fragColor;

void main()
{
	// Ambient lighting calculation
    vec3 ambient = uAmbientStrength * uLightColor;

	// Diffuse lighting calculation
	vec3 norm = normalize(inVertexNormal);
	vec3 lightDir = normalize(uLightPos - inFragPos);

	float diffusionFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffusionFactor * uLightColor;

	// Specular lighting calculation
    vec3 result = (ambient + diffuse) * inVertexColor;
    fragColor = vec4(result, 1.0);
}  