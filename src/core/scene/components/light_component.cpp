#include "light_component.hpp"

using CompType = SceneObjectComponentType;

LightComponent::LightComponent(LightPtr light) :
    SceneObjectComponent(CompType::Light),
    light(light)
{

}

LightComponent::~LightComponent()
{

}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<LightComponent>()
{
    return SceneObjectComponentType::Light;
}

template<>
std::string SceneObjectComponent::componentTypeString<LightComponent>()
{
    return "Light";
}
