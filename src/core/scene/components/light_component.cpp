#include "light_component.hpp"

using CompType = SceneObjectComponentType;

LightComponent::LightComponent(Light light) :
    SceneObjectComponent(CompType::Light),
    LightComponent(light)
{

}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<LightComponent>()
{
    return SceneObjectComponentType::Light;
}
