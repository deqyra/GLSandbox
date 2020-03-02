#ifndef LIGHT_COMPONENT
#define LIGHT_COMPONENT

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../../light.hpp"

class LightComponent : public SceneObjectComponent
{
    public:
        LightComponent(LightPtr light);
        virtual ~LightComponent();

        LightPtr light;
};

template<>
SceneObjectComponentType SceneObjectComponent::componentType<LightComponent>();

template<>
std::string SceneObjectComponent::componentTypeString<LightComponent>();

#endif//LIGHT_COMPONENT