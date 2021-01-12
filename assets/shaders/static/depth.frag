#version 420 core

#include </interface_blocks/vertex_out>
in VertexOut vertOut;

out vec4 fragColor;

uniform float near = 0.1; 
uniform float far  = 100.0; 
  
float linearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    float depth = linearizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(depth), 1.0);
}