#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

layout (location = 0) out vec3 outVertexColor;
layout (location = 1) out vec3 outVertexNormal;
layout (location = 2) out vec2 outTexCoord;
layout (location = 3) out vec3 outFragPos;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);
uniform mat3 normalCorrection = mat3(1.0f);

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
	outVertexColor = inColor;
	outVertexNormal = normalCorrection * inNormal;
	outTexCoord = inTexCoord;
	outFragPos = vec3(view * model * vec4(inPosition, 1.0f));
}