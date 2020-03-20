#ifndef CORE__UBO__MATRIX_UBO_HPP
#define CORE__UBO__MATRIX_UBO_HPP

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

// Proxy class to a UBO resource on the GPU. Dedicated to vertex transformation matrices.9
class MatrixUBO : public UniformBufferObject
{
    public:
        MatrixUBO();

        virtual unsigned int getBindingPoint();
        virtual unsigned int getSize();

        // Set the model matrix in the UBO
        void setModel(glm::mat4 model);
        // Set the view matrix in the UBO
        void setView(glm::mat4 view);
        // Set the projection matrix in the UBO
        void setProjection(glm::mat4 projection);
        // Set the normal matrix in the UBO
        void setNormal(glm::mat4 normal);
};

#endif//CORE__UBO__MATRIX_UBO_HPP