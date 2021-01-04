#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT

#include <renderboi/core/lights/light.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

namespace Renderboi
{

/// @brief Component allowinf to attach any light to a scene object.
class LightComponent : public Component
{
    private:
        LightComponent(LightComponent& other) = delete;
        LightComponent& operator=(const LightComponent& other) = delete;

        /// @brief Pointer to the light used by this component.
        LightPtr _light;

    public:
        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param light Pointer to the light which the component will use.
        ///
        /// @exception If the passed scene object pointer is null, or if the 
        /// passed light pointer is null, the function will throw a 
        /// std::runtime_error.
        LightComponent(const SceneObjectPtr sceneObject, const LightPtr light);

        ~LightComponent();

        /// @brief Get a pointer to the light used by the component.
        ///
        /// @return A pointer to the light used by the component.
        LightPtr getLight() const;
        /// @brief Set the light used by the component.
        ///
        /// @param light Pointer to the light which the component should use.
        ///
        /// @exception If the passed light pointer is null, the function will
        /// throw a std::runtime_error.
        void setLight(const LightPtr light);

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from Component ///
        ///                                   ///
        /////////////////////////////////////////

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @param newParent Pointer the scene object which will be parent to
        /// the cloned component instance.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        LightComponent* clone(const SceneObjectPtr newParent) const override;
};

template<>
ComponentType Component::componentType<LightComponent>();

template<>
std::string Component::componentTypeString<LightComponent>();

template<>
bool Component::multipleInstancesAllowed<LightComponent>();

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT