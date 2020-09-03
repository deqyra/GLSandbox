#ifndef CORE__LIGHTS__TOOLS_HPP
#define CORE__LIGHTS__TOOLS_HPP

// Get light attenuation computation components from a given range
void attenuationFromRange(float range, float& quadraticFactor, float& linearFactor, float& constantFactor);

#endif//CORE__LIGHTS__TOOLS_HPP