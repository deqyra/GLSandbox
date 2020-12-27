#ifndef RENDERBOI__CORE__UBO__MATRIX_UBO_HPP
#define RENDERBOI__CORE__UBO__MATRIX_UBO_HPP

#include <glm/glm.hpp>

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
class MatrixUBO
{
private:
    /// @brief The location (binding point) of the UBO on the GPU.
    unsigned int _location;

public:
    /// @brief The total size, in bytes, of the UBO in the GPU memory.
    static constexpr unsigned int Size         = 240;
    
    /// @brief The total size, in bytes, of the UBO in the GPU memory.
    static constexpr unsigned int BindingPoint = 0;
    
    MatrixUBO();

    /// @brief Set the model matrix in the UBO.
    ///
    /// @param model The model matrix to set in the UBO.
    void setModel(const glm::mat4& model);

    /// @brief Set the view matrix in the UBO.
    ///
    /// @param view The view matrix to set in the UBO.
    void setView(const glm::mat4& view);

    /// @brief Set the projection matrix in the UBO.
    ///
    /// @param projection The projection matrix to set in the UBO.
    void setProjection(const glm::mat4& projection);

    /// @brief Set the normal matrix in the UBO.
    ///
    /// @param normal The normal matrix to set in the UBO. The normal
    /// restoration matrix is supposed to be a 3x3 matrix, but a mat3 will
    /// be aligned to an array of vec4's on the GPU. Since the calculation  
    /// of the normal matrix is performed using 4x4 matrices anyway, the 
    /// result can be directly passed in as a mat4: only the first three 
    /// rows will be sent to the GPU as vec4's, and the last element of 
    /// those will be ignored.
    void setNormal(const glm::mat4& normal);
};

#endif//RENDERBOI__CORE__UBO__MATRIX_UBO_HPP