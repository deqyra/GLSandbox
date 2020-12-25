#include </interface_blocks/vertex_attributes>

#include </interface_blocks/vertex_out>
out VertexOut vertOut;

#include </uniform_blocks/matrices>

void main()
{
#ifdef VERTEX_MVP
	vec4 mvPos = matrices.view * matrices.model * vec4(inPosition, 1.0f);
    gl_Position = matrices.projection * mvPos;
	vertOut.fragPos = vec3(mvPos);
	vertOut.normal = normalize(matrices.normal * inNormal);
#ifdef VERTEX_NORMALS_TO_COLOR
	vertOut.color = vertOut.normal;
#else
	vertOut.color = inColor;
#endif//VERTEX_NORMALS_TO_COLOR
	vertOut.texCoord = inTexCoord;
#endif//VERTEX_MVP
}