#ifndef UNIFORM_BLOCKS__LIGHTS
#define UNIFORM_BLOCKS__LIGHTS

#include </interface_blocks/light_types>

#define POINT_LIGHT_MAX_COUNT 64
#define SPOT_LIGHT_MAX_COUNT 64
#define DIRECTIONAL_LIGHT_MAX_COUNT 4

layout (std140, binding = 1) uniform Lights
{															// Base alignment	// Base offset
	SpotLight spot[SPOT_LIGHT_MAX_COUNT];					// 64 * 96			//     0
	PointLight point[POINT_LIGHT_MAX_COUNT];				// 64 * 80			//  6144
	DirectionalLight direct[DIRECTIONAL_LIGHT_MAX_COUNT];	//  4 * 64			// 11264

	uint pointCount;										// 4				// 11520
	uint spotCount;											// 4				// 11524
	uint directionalCount;									// 4				// 11528
} lights;  													// Size: 11532

#endif//UNIFORM_BLOCKS__LIGHTS