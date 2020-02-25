#ifndef LIGHT_UBO_HPP
#define LIGHT_UBO_HPP

#include <glm/glm.hpp>
#include "../uniform_buffer_object.hpp"
#include "../lights/point_light.hpp"

class LightUBO : public UniformBufferObject
{
    public:
        LightUBO();

        virtual unsigned int getBindingPoint();
        virtual unsigned int getSize();

        void setPoint(unsigned int index, PointLight point);
        void setPointCount(unsigned int count);
};

#endif//LIGHT_UBO_HPP