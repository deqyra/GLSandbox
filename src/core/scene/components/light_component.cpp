#include "light_component.hpp"

using CompType = ComponentType;

LightComponent::LightComponent(LightPtr light) :
    Component(CompType::Light),
    light(light)
{

}

LightComponent::~LightComponent()
{

}

template<>
ComponentType Component::componentType<LightComponent>()
{
    return ComponentType::Light;
}

template<>
std::string Component::componentTypeString<LightComponent>()
{
    return "Light";
}
