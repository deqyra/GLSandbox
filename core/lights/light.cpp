#include "light.hpp"

unsigned int Light::_count = 0;

Light::Light(LightType type) :
    id(_count++),
    lightType(type)
{

}