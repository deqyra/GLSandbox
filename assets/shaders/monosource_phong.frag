#version 460 core
layout (location = 0) in vec3 inVertexColor;
layout (location = 1) in vec2 inTexCoord;

uniform vec3 uLightColor = vec3(1.f);
uniform float uAmbientStrength = 0.2f;

out vec4 fragColor;

void main()
{
    vec3 ambient = uAmbientStrength * uLightColor;
    vec3 result = ambient * inVertexColor;
    fragColor = vec4(result, 1.0);
}  