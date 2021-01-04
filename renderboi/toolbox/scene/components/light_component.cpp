#include "light_component.hpp"

#include <stdexcept>

#include "../scene_object.hpp"

namespace Renderboi
{

LightComponent::LightComponent(const SceneObjectPtr sceneObject, const LightPtr light) :
    Component(ComponentType::Light, sceneObject),
    _light(light)
{
    if (!light)
    {
        throw std::runtime_error("LightComponent: cannot construct from null light pointer.");
    }
}

LightComponent::~LightComponent()
{

}

LightPtr LightComponent::getLight() const
{
    return _light;
}

void LightComponent::setLight(const LightPtr light)
{
    if (!light)
    {
        throw std::runtime_error("LightComponent: cannot set light to null pointer.");
    }

    _light = light;
}

LightComponent* LightComponent::clone(const SceneObjectPtr newParent) const
{
    LightPtr clonedLight = LightPtr(_light->clone());
    return new LightComponent(newParent, clonedLight);
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

template<>
bool Component::multipleInstancesAllowed<LightComponent>()
{
    return false;
}

}//namespace Renderboi
