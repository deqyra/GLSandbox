#ifndef UNIFORM_BUFFER_OBJECT_HPP
#define UNIFORM_BUFFER_OBJECT_HPP

#include <string>
#include <glm/glm.hpp>

class UniformBufferObject
{
    protected:
        unsigned int _id;

    public:
        UniformBufferObject();

        virtual unsigned int getBindingPoint() = 0;
        virtual unsigned int getSize() = 0;
};

#endif//UNIFORM_BUFFER_OBJECT_HPP