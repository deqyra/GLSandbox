#ifndef CORE__SCENE__COMPONENTS__LIGHT_COMPONENT
#define CORE__SCENE__COMPONENTS__LIGHT_COMPONENT

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../lights/light.hpp"

// Component derived class representing any light fixed to a scene object
class LightComponent : public Component
{
    public:
        LightComponent(LightPtr light);
        virtual ~LightComponent();

        // Get a raw pointer to a new LightComponent instance cloned from this. The inner light is cloned as well. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual LightComponent* clone();

        // Pointer to the actual light represented by this component
        LightPtr light;
};

template<>
ComponentType Component::componentType<LightComponent>();

template<>
std::string Component::componentTypeString<LightComponent>();

#endif//CORE__SCENE__COMPONENTS__LIGHT_COMPONENT