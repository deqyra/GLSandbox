#include "light.hpp"

unsigned int Light::_count = 0;

Light::Light(const LightType type) :
    id(_count++),
    lightType(type)
{

}