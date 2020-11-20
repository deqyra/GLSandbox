#ifndef UNIFORM_BLOCKS__MATERIAL
#define UNIFORM_BLOCKS__MATERIAL

#define DIFFUSE_MAP_MAX_COUNT   8
#define SPECULAR_MAP_MAX_COUNT  8

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

uniform Material material;

#endif//UNIFORM_BLOCKS__MATERIAL