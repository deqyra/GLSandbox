#include "light_component.hpp"

#include <stdexcept>

#include "../scene_object.hpp"

LightComponent::LightComponent(LightPtr light) :
    Component(ComponentType::Light)
{
    setLight(light);
}

LightComponent::~LightComponent()
{

}

LightPtr LightComponent::getLight()
{
    return _light;
}

void LightComponent::setLight(LightPtr light)
{
    if (!light)
    {
        std::string s = "NONE";
        std::shared_ptr<SceneObject> sceneObject = _sceneObject.lock();
        if (sceneObject) s = std::to_string(sceneObject->id);

        s = "LightComponent (on scene object with ID " + s + ") was passed a null light pointer.";
        throw std::runtime_error(s.c_str());
    }

    _light = light;
}

LightComponent* LightComponent::clone()
{
    // This shared pointer will be destroyed at the end of this scope, but responsibilty for the 
    // resources will already have been shared with the cloned CameraComponent by this point.
    LightPtr clonedLight = LightPtr(_light->clone());
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

template<>
bool Component::multipleInstancesAllowed<LightComponent>()
{
    return false;
}
