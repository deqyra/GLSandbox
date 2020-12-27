#ifndef LIGHT_UBO_HPP
#define LIGHT_UBO_HPP

#include <glm/glm.hpp>

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
 *     unsigned int pointCount;                                //  4               // 11520
 *     unsigned int spotCount;                                 //  4               // 11524
 *     unsigned int directionalCount;                          //  4               // 11528
 * };                                                          // Size: 11532
 **/

class LightUBO
{
private:
    /// @brief The location (binding point) of the UBO on the GPU.
    unsigned int _location;

public:
    /// @brief The maximum count of spot lights in the UBO. 
    static constexpr unsigned int SpotLightMaxCount         = 64;
    
    /// @brief The size of a spot light in the UBO, in bytes.
    static constexpr unsigned int SpotLightSize             = 96;
    
    /// @brief The offset of the first spot light in the UBO.
    static constexpr unsigned int SpotLightOffset           = 0;
    
    /// @brief The maximum count of point lights in the UBO. 
    static constexpr unsigned int PointLightMaxCount        = 64;
    
    /// @brief The size of a point light in the UBO, in bytes.
    static constexpr unsigned int PointLightSize            = 80;
    
    /// @brief The offset of the first point light in the UBO.
    static constexpr unsigned int PointLightOffset          = 6144;
    
    /// @brief The maximum count of directional lights in the UBO. 
    static constexpr unsigned int DirectionalLightMaxCount  = 4;
    
    /// @brief The size of a directional light in the UBO, in bytes.
    static constexpr unsigned int DirectionalLightSize      = 64;
    
    /// @brief The offset of the first directional light in the UBO.
    static constexpr unsigned int DirectionalLightOffset    = 11264;
    
    /// @brief The offset of the light counts in the UBO.
    static constexpr unsigned int LightCountsOffset         = 11520;
    
    /// @brief The total size, in bytes, of the UBO in the GPU memory.
    static constexpr unsigned int Size                      = 11532;
    
    /// @brief The total size, in bytes, of the UBO in the GPU memory.
    static constexpr unsigned int BindingPoint              = 1;

    LightUBO();

    /// @brief Set a point light in the UBO.
    ///
    /// @param index The index of the point light to set in the UBO.
    /// @param point The point light to send to the UBO.
    /// @param position The position of the point light to send to the UBO.
    void setPoint(const unsigned int index, const PointLight& point, const glm::vec3& position);

    /// @brief Set the number of point lights in the UBO.
    ///
    /// @param count The number of point lights in the UBO.
    void setPointCount(const unsigned int count);

    /// @brief Set a spot light in the UBO.
    ///
    /// @param index The index of the spot light to set in the UBO.
    /// @param spot The spot light to send to the UBO.
    /// @param position The position of the spot light to send to the UBO.
    void setSpot(const unsigned int index, const SpotLight& spot, const glm::vec3& position);

    /// @brief Set the number of spot lights in the UBO.
    ///
    /// @param count The number of spot lights in the UBO.
    void setSpotCount(const unsigned int count);

    /// @brief Set a directional light in the UBO.
    ///
    /// @param index The index of the directional light to set in the UBO.
    /// @param light The directional light to send to the UBO.
    void setDirectional(const unsigned int index, const DirectionalLight& light);

    /// @brief Set the number of directional lights in the UBO.
    ///
    /// @param count The number of directional lights in the UBO.
    void setDirectionalCount(const unsigned int count);
};

#endif//LIGHT_UBO_HPP