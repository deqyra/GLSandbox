#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

out VertexOut 
{
	vec3 color;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
} vertOut;

layout (std140, binding = 0) uniform matrices
{						// Base alignment	// Base offset
	mat4 model;			// 64				//   0
	mat4 view;			// 64				//  64
	mat4 projection;	// 64				// 128
	mat3 normal;		// 64				// 192
};						// Size: 256

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
	vertOut.color = inColor;
	vertOut.normal = normal * inNormal;
	vertOut.texCoord = inTexCoord;
	vertOut.fragPos = vec3(view * model * vec4(inPosition, 1.0f));
}