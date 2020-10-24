#define POINT_LIGHT_MAX_COUNT 64
#define SPOT_LIGHT_MAX_COUNT 64
#define DIRECTIONAL_LIGHT_MAX_COUNT 4

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

layout (std140, binding = 1) uniform Lights
{															// Base alignment	// Base offset
	SpotLight spot[SPOT_LIGHT_MAX_COUNT];					// 64 * 96			//     0
	PointLight point[POINT_LIGHT_MAX_COUNT];				// 64 * 80			//  6144
	DirectionalLight direct[DIRECTIONAL_LIGHT_MAX_COUNT];	//  4 * 64			// 11264

	uint pointCount;										// 4				// 11520
	uint spotCount;											// 4				// 11524
	uint directionalCount;									// 4				// 11528
} lights;  													// Size: 11532
