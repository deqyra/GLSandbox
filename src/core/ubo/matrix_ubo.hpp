#ifndef MATRIX_UBO_HPP
#define MATRIX_UBO_HPP

#include <glm/glm.hpp>

#include "../uniform_buffer_object.hpp"

/* UNIFORM BLOCK LAYOUT
 * ====================
 *
 * layout (std140, binding = 0) uniform matrices
 * {						// Base alignment	// Base offset
 *     mat4 model;			// 64				//   0
 *     mat4 view;			// 64				//  64
 *     mat4 projection;	    // 64				// 128
 *     mat3 normal;		    // 48				// 192
 * };						// Size: 240
 **/

class MatrixUBO : public UniformBufferObject
{
    public:
        MatrixUBO();

        virtual unsigned int getBindingPoint();
        virtual unsigned int getSize();

        void setModel(glm::mat4 model);
        void setView(glm::mat4 view);
        void setProjection(glm::mat4 projection);
        void setNormal(glm::mat4 normal);
};

#endif//MATRIX_UBO_HPP