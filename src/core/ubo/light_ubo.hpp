#ifndef LIGHT_UBO_HPP
#define LIGHT_UBO_HPP

#include <glm/glm.hpp>
#include "../uniform_buffer_object.hpp"
#include "../lights/point_light.hpp"

/* UNIFORM BLOCK LAYOUT
 * ====================
 *
 * layout (std140, binding = 1) uniform lights
 * {											// Base alignment	// Base offset
 *     PointLight point[POINT_LIGHT_MAX_COUNT]; // 64 * 128			//    0
 *     unsigned int nPoint;		    			// 4				// 8192
 * };											// Size: 8196
 **/

class LightUBO : public UniformBufferObject
{
    public:
        LightUBO();

        virtual unsigned int getBindingPoint();
        virtual unsigned int getSize();

        void setPoint(unsigned int index, PointLight point, glm::vec3 position);
        void setPointCount(unsigned int count);
};

#endif//LIGHT_UBO_HPP