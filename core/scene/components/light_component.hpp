#ifndef CORE__SCENE__COMPONENTS__LIGHT_COMPONENT
#define CORE__SCENE__COMPONENTS__LIGHT_COMPONENT

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../lights/light.hpp"

/// @brief Component allowinf to attach any light to a scene object.
class LightComponent : public Component
{
    private:
        LightComponent(LightComponent& other) = delete;
        LightComponent& operator=(const LightComponent& other) = delete;

        /// @brief Pointer to the light used by this component.
        LightPtr _light;

    public:
        /// @param light Pointer to the light which the component will use.
        LightComponent(LightPtr light);

        virtual ~LightComponent();

        /// @brief Get a pointer to the light used by the component.
        ///
        /// @return A pointer to the light used by the component.
        LightPtr getLight();

        /// @brief Set the light used by the component.
        ///
        /// @param light Pointer to the light which the component should use.
        ///
        /// @exception If the passed light pointer is null, the function will
        /// throw a std::runtime_error.
        void setLight(LightPtr light);

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual LightComponent* clone();
};

template<>
ComponentType Component::componentType<LightComponent>();

template<>
std::string Component::componentTypeString<LightComponent>();

#endif//CORE__SCENE__COMPONENTS__LIGHT_COMPONENT