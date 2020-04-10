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

LightComponent* LightComponent::clone()
{
    // This shared pointer will be destroyed at the end of this scope, but responsibilty for the 
    // resources will already have been shared with the cloned CameraComponent by this point.
    LightPtr clonedLight = LightPtr(light->clone());
    return new LightComponent(clonedLight);
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
