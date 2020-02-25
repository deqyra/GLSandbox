#version 460 core
in VertexOut 
{
	vec3 color;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
} vertOut;

out vec4 fragColor;

// Types and constants
// ===================

struct PointLight
{											// Base alignment	// Base offset
    vec3 position;							// 16				//  0
    vec3 ambient;							// 16				// 16
    vec3 diffuse;							// 16				// 32
    vec3 specular;							// 16				// 48
};											// Size: 64

struct Material
{											// Base alignment	// Base offset
    vec3 ambient;							// 16				// 0
    vec3 diffuse;							// 16				// 16
    vec3 specular;							// 16				// 32
    float shininess;						// 4				// 48
};											// Size: 52

#define POINT_LIGHT_MAX_COUNT 128

// Uniforms
// ========

layout (std140, binding = 1) uniform lights
{											// Base alignment	// Base offset
	PointLight point[POINT_LIGHT_MAX_COUNT];// 64 * 128			//    0
	unsigned int nPoint;					// 4				// 8192
};											// Size: 8196

uniform Material material;

// Functions
// =========

vec3 processPointLight(int i);

void main()
{
	vec3 pLightTotal = vec3(0.f);
	for (int i = 0; i < nPoint; i++)
	{
		pLightTotal += processPointLight(i);
	}

    vec3 result = pLightTotal * vertOut.color;
    fragColor = vec4(result, 1.0);
}

vec3 processPointLight(int i)
{
	vec3 normal = normalize(vertOut.normal);
	vec3 viewDir = normalize(vertOut.fragPos);
	vec3 lightDir = normalize(vertOut.fragPos - point[i].position);

	// Ambient lighting
    vec3 ambient = point[i].ambient * material.ambient;

	// Diffuse lighting
	float diffusionFactor = max(dot(normal, -lightDir), 0.0);
	vec3 diffuse = point[i].diffuse * material.diffuse * diffusionFactor;

	// Specular
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = point[i].specular * material.specular * spec;

	return ambient + diffuse + specular;
}