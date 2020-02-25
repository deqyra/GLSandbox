#ifndef MATRIX_UBO_HPP
#define MATRIX_UBO_HPP

#include <glm/glm.hpp>

#include "../uniform_buffer_object.hpp"

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