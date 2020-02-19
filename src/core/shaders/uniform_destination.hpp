#ifndef UNIFORM_DESTINATION_HPP
#define UNIFORM_DESTINATION_HPP

enum class UniformDestination
{
    Time,
    ModelMatrix,
    ViewMatrix,
    ProjectionMatrix,
    NormalMatrix,
    Material,
    MaxTextureCount,
    Texture1Data,
    Texture2Data,
    TextureMix,
    PointLightCount,
    PointLightArray,
    SpotLightCount,
    SpotLightArray,
    DirectionalLightCount,
    DirectionalLightArray
};

#endif//UNIFORM_DESTINATION_HPP