#ifndef RENDERBOI__CORE__UBO__UNIFORM_BUFFER_OBJECT_HPP
#define RENDERBOI__CORE__UBO__UNIFORM_BUFFER_OBJECT_HPP

#include <string>
#include <glm/glm.hpp>

/// @brief Manager for a uniform buffer object resource on the GPU.
class UniformBufferObject
{
    protected:
        /// @brief The location (binding point) of the UBO on the GPU.
        unsigned int _location;

    public:
        UniformBufferObject();

        /// @brief Get the binding point of the UBO on the GPU.
        ///
        /// @return The binding point of the UBO on the GPU.
        virtual unsigned int getBindingPoint() = 0;

        /// @brief Get the size the UBO takes in GPU memory.
        ///
        /// @return The amount of GPU memory occupied by the UBO, in bytes.
        virtual unsigned int getSize() = 0;
};

#endif//RENDERBOI__CORE__UBO__UNIFORM_BUFFER_OBJECT_HPP