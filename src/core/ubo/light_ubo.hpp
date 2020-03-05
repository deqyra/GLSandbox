#ifndef LIGHT_UBO_HPP
#define LIGHT_UBO_HPP

#include <glm/glm.hpp>
#include "../uniform_buffer_object.hpp"
#include "../lights/point_light.hpp"
#include "../lights/directional_light.hpp"
#include "../lights/spot_light.hpp"

/* UNIFORM BLOCK LAYOUT
 * ====================
 *
 * layout (std140, binding = 1) uniform lights
 * {                                                           // Base alignment   // Base offset
 *     PointLight point[POINT_LIGHT_MAX_COUNT];                // 64 *  80         //    0
 *     SpotLight spot[SPOT_LIGHT_MAX_COUNT];                   // 64 * 112         // 5120
 *     DirectionalLight direct[DIRECTIONAL_LIGHT_MAX_COUNT];   //  4 *  64         // 9216
 *     unsigned int pointCount;                                // 4                // 9728
 *     unsigned int spotCount;                                 // 4                // 9732
 *     unsigned int directionalCount;                          // 4                // 9736
 * };                                                          // Size: 9740
 **/

class LightUBO : public UniformBufferObject
{
    public:
        LightUBO();

        virtual unsigned int getBindingPoint();
        virtual unsigned int getSize();

        void setPoint(unsigned int index, PointLight point, glm::vec3 position);
        void setPointCount(unsigned int count);

        void setSpot(unsigned int index, SpotLight spot, glm::vec3 position);
        void setSpotCount(unsigned int count);

        void setDirectional(unsigned int index, DirectionalLight light);
        void setDirectionalCount(unsigned int count);
};

#endif//LIGHT_UBO_HPP