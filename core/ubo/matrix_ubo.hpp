#ifndef CORE__UBO__MATRIX_UBO_HPP
#define CORE__UBO__MATRIX_UBO_HPP

#include <glm/glm.hpp>

#include "uniform_buffer_object.hpp"

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

/// @brief Manager for a UBO resource on the GPU, meants for vertex 
/// transformation matrices.
class MatrixUBO : public UniformBufferObject
{
    public:
        MatrixUBO();

        /// @brief Get the binding point of the UBO on the GPU.
        ///
        /// @return The binding point of the UBO on the GPU.
        virtual unsigned int getBindingPoint();

        /// @brief Get the size the UBO takes in GPU memory.
        ///
        /// @return The amount of GPU memory occupied by the UBO, in bytes.
        virtual unsigned int getSize();

        /// @brief Set the model matrix in the UBO.
        ///
        /// @param model The model matrix to set in the UBO.
        void setModel(glm::mat4 model);

        /// @brief Set the view matrix in the UBO.
        ///
        /// @param view The view matrix to set in the UBO.
        void setView(glm::mat4 view);

        /// @brief Set the projection matrix in the UBO.
        ///
        /// @param projection The projection matrix to set in the UBO.
        void setProjection(glm::mat4 projection);

        /// @brief Set the normal matrix in the UBO.
        ///
        /// @param normal The normal matrix to set in the UBO.
        void setNormal(glm::mat4 normal);
};

#endif//CORE__UBO__MATRIX_UBO_HPP