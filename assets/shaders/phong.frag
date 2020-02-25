#version 460 core
in VertexOut 
{
	vec3 color;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
} fragIn;

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

#define MAX_POINT_LIGHTS 128

// Uniforms
// ========

layout (std140, binding = 0) uniform matrices
{											// Base alignment	// Base offset
	mat4 model;								// 64				//   0
	mat4 view;								// 64				//  64
	mat4 projection;						// 64				// 128
	mat3 normal;							// 64				// 192
};											// Size: 256

layout (std140, binding = 1) uniform lights
{											// Base alignment	// Base offset
	PointLight point[MAX_POINT_LIGHTS];		// 64 * 128			//    0
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

    vec3 result = pLightTotal * fragIn.color;
    fragColor = vec4(result, 1.0);
}

vec3 processPointLight(int i)
{
	vec3 normal = normalize(fragIn.normal);
	vec3 viewDir = normalize(fragIn.fragPos);
	vec3 lightDir = normalize(fragIn.fragPos - point[i].position);

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