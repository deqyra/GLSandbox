#ifndef INTERFACE_BLOCKS__LIGHT_TYPES
#define INTERFACE_BLOCKS__LIGHT_TYPES

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

#endif//INTERFACE_BLOCKS__LIGHT_TYPES