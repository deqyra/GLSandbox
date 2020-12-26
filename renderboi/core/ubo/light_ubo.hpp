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

class LightUBO : public UniformBufferObject
{
    public:
        /// @brief The maximum count of spot lights in the UBO. 
        static constexpr unsigned int SpotLightMaxCount          = 64;
        
        /// @brief The size of a spot light in the UBO, in bytes.
        static constexpr unsigned int SpotLightUboSize           = 96;
        
        /// @brief The offset of the first spot light in the UBO.
        static constexpr unsigned int SpotLightUboOffset         = 0;
        
        /// @brief The maximum count of point lights in the UBO. 
        static constexpr unsigned int PointLightMaxCount         = 64;
        
        /// @brief The size of a point light in the UBO, in bytes.
        static constexpr unsigned int PointLightUboSize          = 80;
        
        /// @brief The offset of the first point light in the UBO.
        static constexpr unsigned int PointLightUboOffset        = 6144;
        
        /// @brief The maximum count of directional lights in the UBO. 
        static constexpr unsigned int DirectionalLightMaxCount   = 4;
        
        /// @brief The size of a directional light in the UBO, in bytes.
        static constexpr unsigned int DirectionalLightUboSize    = 64;
        
        /// @brief The offset of the first directional light in the UBO.
        static constexpr unsigned int DirectionalLightUboOffset  = 11264;
        
        /// @brief The offset of the light counts in the UBO.
        static constexpr unsigned int LightUboCountsOffset       = 11520;
        
        /// @brief The total size, in bytes, of the UBO in the GPU memory.
        static constexpr unsigned int LightUboSize                = 11532;

        LightUBO();

        /// @brief Set a point light in the UBO.
        ///
        /// @param index The index of the point light to set in the UBO.
        /// @param point The point light to send to the UBO.
        /// @param position The position of the point light to send to the UBO.
        void setPoint(unsigned int index, PointLight point, glm::vec3 position);

        /// @brief Set the number of point lights in the UBO.
        ///
        /// @param count The number of point lights in the UBO.
        void setPointCount(unsigned int count);

        /// @brief Set a spot light in the UBO.
        ///
        /// @param index The index of the spot light to set in the UBO.
        /// @param spot The spot light to send to the UBO.
        /// @param position The position of the spot light to send to the UBO.
        void setSpot(unsigned int index, SpotLight spot, glm::vec3 position);

        /// @brief Set the number of spot lights in the UBO.
        ///
        /// @param count The number of spot lights in the UBO.
        void setSpotCount(unsigned int count);

        /// @brief Set a directional light in the UBO.
        ///
        /// @param index The index of the directional light to set in the UBO.
        /// @param light The directional light to send to the UBO.
        void setDirectional(unsigned int index, DirectionalLight light);

        /// @brief Set the number of directional lights in the UBO.
        ///
        /// @param count The number of directional lights in the UBO.
        void setDirectionalCount(unsigned int count);

        //////////////////////////////////////////////////
        ///                                            ///
        /// Methods overridden from UniformBufferObjet ///
        ///                                            ///
        //////////////////////////////////////////////////

        /// @brief Get the binding point of the UBO on the GPU.
        ///
        /// @return The binding point of the UBO on the GPU.
        virtual unsigned int getBindingPoint();

        /// @brief Get the size the UBO takes in GPU memory.
        ///
        /// @return The amount of GPU memory occupied by the UBO, in bytes.
        virtual unsigned int getSize();
};

#endif//LIGHT_UBO_HPP