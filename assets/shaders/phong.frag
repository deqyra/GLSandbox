#version 420 core
in VertexOut 
{
	vec3 fragPos;
	vec3 color;
	vec3 normal;
	vec2 texCoord;
} vertOut;

out vec4 fragColor;

// Types and constants
// ===================

#define POINT_LIGHT_MAX_COUNT 64
#define SPOT_LIGHT_MAX_COUNT 64
#define DIRECTIONAL_LIGHT_MAX_COUNT 4
#define DIFFUSE_MAP_MAX_COUNT   8
#define SPECULAR_MAP_MAX_COUNT  8

struct PointLight
{											// Base alignment	// Base offset
    vec3 position;							// 16				//  0
    vec3 ambient;							// 16				// 16
    vec3 diffuse;							// 16				// 32
    vec3 specular;							// 12				// 48
    float constant;							//  4				// 60
    float linear;							//  4				// 64
    float quadratic;  						//  4				// 68
};											// Size: 80 = 72 + vec4 padding

struct SpotLight
{											// Base alignment	// Base offset
	vec3 position;							// 16				//  0
	vec3 direction;							// 16				// 16
    vec3 ambient;							// 16				// 32
    vec3 diffuse;							// 16				// 48
    vec3 specular;							// 12				// 64
    float constant;							//  4				// 76
    float linear;							//  4				// 80
    float quadratic;  						//  4				// 84
	float innerCutoff;						//  4				// 88
	float outerCutoff;						//  4				// 92
};											// Size: 96

struct DirectionalLight
{											// Base alignment	// Base offset
    vec3 direction;							// 16				//  0
    vec3 ambient;							// 16				// 16
    vec3 diffuse;							// 16				// 32
    vec3 specular;							// 16				// 48
};											// Size: 64

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

	sampler2D diffuseMaps[DIFFUSE_MAP_MAX_COUNT];
	sampler2D specularMaps[SPECULAR_MAP_MAX_COUNT];

	uint diffuseMapCount;
	uint specularMapCount;
};

// Uniforms
// ========

layout (std140, binding = 1) uniform lights
{															// Base alignment	// Base offset
	SpotLight spot[SPOT_LIGHT_MAX_COUNT];					// 64 * 96			//     0
	PointLight point[POINT_LIGHT_MAX_COUNT];				// 64 * 80			//  6144
	DirectionalLight direct[DIRECTIONAL_LIGHT_MAX_COUNT];	//  4 * 64			// 11264

	uint pointCount;										// 4				// 11520
	uint spotCount;											// 4				// 11524
	uint directionalCount;									// 4				// 11528
};															// Size: 11532

uniform Material material;

// Functions
// =========

vec4 processPointLight(int i);
vec4 processSpotLight(int i);
vec4 processDirectionalLight(int i);
vec4 calculatePhong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight);

void main()
{
	// Process all point lights
	vec4 lightTotal = vec4(0.f);
	for (int i = 0; i < pointCount; i++)
	{
		lightTotal += processPointLight(i);
	}
	for (int i = 0; i < spotCount; i++)
	{
		lightTotal += processSpotLight(i);
	}
	for (int i = 0; i < directionalCount; i++)
	{
		lightTotal += processDirectionalLight(i);
	}	

	// Combine components together
    fragColor = lightTotal * vec4(vertOut.color, 1.f);
}

vec4 processPointLight(int i)
{
	vec3 positionDiff = vertOut.fragPos - point[i].position;
	vec3 lightDirection = normalize(positionDiff);
	float dist = length(positionDiff);
	// float attenuation = 1.f;
	float attenuation = 1.f / (point[i].constant + (point[i].linear * dist) + (point[i].quadratic * dist * dist));

	return attenuation * calculatePhong(lightDirection, point[i].ambient, point[i].diffuse, point[i].specular);
}

vec4 processSpotLight(int i)
{
	vec3 positionDiff = vertOut.fragPos - spot[i].position;
	vec3 lightDirection = normalize(positionDiff);

	float theta = dot(lightDirection, normalize(spot[i].direction));
	float outerCos = cos(spot[i].outerCutoff);
	float innerCos = cos(spot[i].innerCutoff);
	float epsilon = innerCos - outerCos;
	float intensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);

	float dist = length(positionDiff);
	float attenuation = 1.0 / (spot[i].constant + (spot[i].linear * dist) + (spot[i].quadratic * dist * dist));

	return attenuation * intensity * calculatePhong(lightDirection, spot[i].ambient, spot[i].diffuse, spot[i].specular);
}

vec4 processDirectionalLight(int i)
{
	return calculatePhong(direct[i].direction, direct[i].ambient, direct[i].diffuse, direct[i].specular);
}

vec4 calculatePhong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight)
{
	vec3 normal = normalize(vertOut.normal);
	vec3 viewDir = normalize(vertOut.fragPos);
	vec3 reflectDir = reflect(lightDirection, normal);

	// Ambient lighting
    vec4 ambient = vec4(ambientLight, 1.f) * vec4(material.ambient, 1.f);

	// Diffuse lighting
	vec4 diffuseTexel = vec4(1.f);
	if (material.diffuseMapCount > 0)
		diffuseTexel = texture(material.diffuseMaps[0], vertOut.texCoord);
	
	float diffusionFactor = max(dot(normal, -lightDirection), 0.0);
	vec4 diffuse = vec4(diffuseLight, 1.f) * vec4(material.diffuse, 1.f) * diffuseTexel * diffusionFactor;

	// Specular
	vec4 specularTexel = vec4(1.f);
	if (material.specularMapCount > 0)
		specularTexel = texture(material.specularMaps[0], vertOut.texCoord);

	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = vec4(specularLight, 1.f) * vec4(material.specular, 1.f) * specularTexel * spec;

	return ambient + diffuse + specular;
}
