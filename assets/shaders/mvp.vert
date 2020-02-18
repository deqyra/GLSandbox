#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

layout (location = 0) out vec3 outVertexColor;
layout (location = 1) out vec3 outVertexNormal;
layout (location = 2) out vec2 outTexCoord;
layout (location = 3) out vec3 outFragPos;

uniform mat4 uModel = mat4(1.0f);
uniform mat4 uView = mat4(1.0f);
uniform mat4 uProjection = mat4(1.0f);
uniform mat4 uNormalCorrection = mat4(1.0f);

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0f);
	outVertexColor = inColor;
	outVertexNormal = vec3(uNormalCorrection * vec4(inNormal, 0.0f));
	outTexCoord = inTexCoord;
	outFragPos = vec3(uModel * vec4(inPosition, 1.0f));
}