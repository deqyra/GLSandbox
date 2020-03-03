#ifndef UBO_BINDING_POINTS_HPP
#define UBO_BINDING_POINTS_HPP

/* UNIFORM BLOCK LAYOUT RULE
 * =========================
 * - All members have a base alignment and a base offset.
 *   The base offset of a member is the sum of the base offset and size of the previous element.
 *   The base offset *must* be a multiple of the base alignment, and so members should be declared from largest to smallest.
 *
 * - Scalars (int, float, bool...) have a base alignment of 4.
 *
 * - Vectors have a base alignment of 8 or 16 depending on what their elements fit in.
 *   For vec2 it's 8, for both vec3 and vec4 it's 16.
 *
 * - Arrays of scalars or vectors: each element has the base alignment of a vec4.
 *   The base alignement of the whole array is 16 * nElements.
 *
 * - Matrices are considered to be an array of column vectors, each of which has the base alignment of a vec4.
 *   A mat4 is an array of four vec4. The base alignment of a mat4 is 64.
 *   A mat3 is an array of three vec3. The base alignment of a mat4 is 48.
 *   A mat2 is an array of two vec2. The base alignment of a mat2 is 32 (not 16 as the inner vec2s are aligned to vec4s).
 *   This misalignment of vectors indicates that matrix elements are not stored contiguously,
 *   and one must be cautious when copying contents over to the UBO.
 *
 * - Structs have a base alignment computed from all of the above rules applied to their members,
 *   and they are padded to be a multiple of the base alignment of a vec4.
 *
 * EXAMPLES
 * ========
 * layout (std140) uniform example
 * {                // Inner elt.   // Base alignment       // Base offset
 *     mat3 test;   // test[0]      // 16                   // 0
 *                  // test[1]      // 16                   // 16
 *                  // test[2]      // 16                   // 32
 * };               // Size: 48
 *
 * struct MyStruct
 * {                                // Base alignment       // Base offset
 *     mat3 myArray[4];             // 48 * 4               // 0
 *     mat4 myMat;                  // 64                   // 192
 *     vec3 myVec;                  // 16                   // 256
 *     int myInt;                   // 4                    // 260
 * };                               // Size: 264 + vec4 padding = 272
 *
 * layout (std140) uniform example
 * {                                // Base alignment       // Base offset
 *     MyStruct test1[2];           // 544 = 272 * 2        // 0
 *     mat3 test2;                  // 48                   // 576 = 544 + 48-padding
 *     vec3 test3;                  // 16                   // 624
 *     int test4;                   // 4                    // 640
 *     float test5;                 // 4                    // 644
 *     bool test6;                  // 4                    // 648
 * };                               // Size: 652
 **/

#define MATRIX_UBO_BINDING_POINT   0
#define LIGHT_UBO_BINDING_POINT    1

#endif//UBO_BINDING_POINTS_HPP