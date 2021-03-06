#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include <string>

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../script.hpp"

namespace Renderboi
{

/// @brief Component allowing a scene object to self-update through time.
class ScriptComponent : public Component
{
    private:
        ScriptComponent(ScriptComponent& other) = delete;
        ScriptComponent& operator=(const ScriptComponent& other) = delete;

        /// @brief Pointer to the actual script resource.
        ScriptPtr _script;

        /// @brief Register script to the scene which the parent scene object
        /// belongs to.
        void _registerScript();

        /// @brief Detach script from the scene it is registered at.
        void _detachScript();

        /// @brief Release the parent scene object pointer.
        void _releaseSceneObject();

    public:
        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param script Pointer to the input processing script which the 
        /// component will use.
        ///
        /// @exception If the passed scene object pointer is null, or if the 
        /// passed script pointer is null, the function will throw a 
        /// std::runtime_error.
        ScriptComponent(const SceneObjectPtr sceneObject, const ScriptPtr script);

        ~ScriptComponent();

        /// @brief Get a pointer to the script used by the component.
        ///
        /// @return Pointer to the script used by the component.
        ScriptPtr getScript() const;

        /// @brief Set the script used by the component.
        ///
        /// @param script Pointer to the script which the component will use.
        ///
        /// @exception If the passed script pointer is null, the function will
        /// throw a std::runtime_error.
        void setScript(const ScriptPtr script);

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
        ScriptComponent* clone(const SceneObjectPtr newParent) const override;
};

template<>
ComponentType Component::componentType<ScriptComponent>();

template<>
std::string Component::componentTypeString<ScriptComponent>();

template<>
bool Component::multipleInstancesAllowed<ScriptComponent>();

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP