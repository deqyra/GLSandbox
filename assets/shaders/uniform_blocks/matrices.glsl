#ifndef UNIFORM_BLOCKS__MATRICES
#define UNIFORM_BLOCKS__MATRICES

layout (std140, binding = 0) uniform Matrices
{						// Base alignment	// Base offset
	mat4 model;			// 64				//   0
	mat4 view;			// 64				//  64
	mat4 projection;	// 64				// 128
	mat3 normal;		// 48				// 192
} matrices; 			// Size: 240

#endif//UNIFORM_BLOCKS__MATRICES