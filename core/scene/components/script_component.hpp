#ifndef CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../script.hpp"

#include <string>

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
        void registerScript();

        /// @brief Detach script from the scene it is registered at.
        void detachScript();

    public:
        /// @param script Pointer to the input processing script which the 
        /// component will use.
        ScriptComponent(ScriptPtr script);

        virtual ~ScriptComponent();

        /// @brief Get a pointer to the script used by the component.
        ///
        /// @return Pointer to the script used by the component.
        ScriptPtr getScript();

        /// @brief Set the script used by the component.
        ///
        /// @param script Pointer to the script which the component will use.
        void setScript(ScriptPtr script);

        /// @brief Set the parent scene object to this component..
        ///
        /// @param sceneObject A pointer to the new parent scene object of this
        /// component.
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual ScriptComponent* clone();
};

template<>
ComponentType Component::componentType<ScriptComponent>();

template<>
std::string Component::componentTypeString<ScriptComponent>();

#endif//CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP