#ifndef CORE__UNIFORM_BUFFER_OBJECT_HPP
#define CORE__UNIFORM_BUFFER_OBJECT_HPP

#include <string>
#include <glm/glm.hpp>

// Abstract proxy class to a uniform buffer object resource on the GPU
class UniformBufferObject
{
    protected:
        unsigned int _location;

    public:
        UniformBufferObject();

        virtual unsigned int getBindingPoint() = 0;
        virtual unsigned int getSize() = 0;
};

#endif//CORE__UNIFORM_BUFFER_OBJECT_HPP