#ifndef CORE__LIGHTS__TOOLS_HPP
#define CORE__LIGHTS__TOOLS_HPP

/// @brief Get components for light attenuation computation from a given range.
///
/// @param[in] range The desired perceived range of the light.
/// @param[out] quadraticFactor Will receive the quadratic component of the
/// light attenuation.
/// @param[out] linearFactor Will receive the linear component of the
/// light attenuation.
/// @param[out] constantFactor Will receive the constant component of the
/// light attenuation.
void attenuationFromRange(float range, float& quadraticFactor, float& linearFactor, float& constantFactor);

#endif//CORE__LIGHTS__TOOLS_HPP