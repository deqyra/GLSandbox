#ifndef LIGHT_COMPONENT
#define LIGHT_COMPONENT

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../../light.hpp"

class LightComponent : public Component
{
    public:
        LightComponent(LightPtr light);
        virtual ~LightComponent();

        LightPtr light;
};

template<>
ComponentType Component::componentType<LightComponent>();

template<>
std::string Component::componentTypeString<LightComponent>();

#endif//LIGHT_COMPONENT