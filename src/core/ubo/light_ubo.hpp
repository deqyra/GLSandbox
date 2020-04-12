#ifndef LIGHT_UBO_HPP
#define LIGHT_UBO_HPP

#include <glm/glm.hpp>

#include "uniform_buffer_object.hpp"
#include "../lights/point_light.hpp"
#include "../lights/directional_light.hpp"
#include "../lights/spot_light.hpp"

/* UNIFORM BLOCK LAYOUT
 * ====================
 *
 * layout (std140, binding = 1) uniform lights
 * {                                                           // Base alignment   // Base offset
 *     SpotLight spot[SPOT_LIGHT_MAX_COUNT];                   // 64 * 96          //     0
 *     PointLight point[POINT_LIGHT_MAX_COUNT];                // 64 * 80          //  6144
 *     DirectionalLight direct[DIRECTIONAL_LIGHT_MAX_COUNT];   //  4 * 64          // 11264
 *     unsigned int pointCount;                                // 4                // 11520
 *     unsigned int spotCount;                                 // 4                // 11524
 *     unsigned int directionalCount;                          // 4                // 11528
 * };                                                          // Size: 11532
 **/

#define SPOT_LIGHT_MAX_COUNT  64
#define SPOT_LIGHT_UBO_SIZE   96
#define SPOT_LIGHT_UBO_OFFSET 0

#define POINT_LIGHT_MAX_COUNT 64
#define POINT_LIGHT_UBO_SIZE  80
#define POINT_LIGHT_UBO_OFFSET 6144

#define DIRECTIONAL_LIGHT_MAX_COUNT 4
#define DIRECTIONAL_LIGHT_UBO_SIZE  64
#define DIRECTIONAL_LIGHT_UBO_OFFSET 11264

#define LIGHT_UBO_COUNTS_OFFSET 11520

#define LIGHT_UBO_SIZE 11532

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