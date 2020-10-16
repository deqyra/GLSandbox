#ifndef CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../input_processing_script.hpp"

#include <string>

/// @brief Component allowing a scene object to process input events.
class InputProcessingScriptComponent : public Component
{
    private:
        /// @brief Pointer to the actual script resource.
        InputProcessingScriptPtr _script;

        /// @brief Register script to the scene which the parent scene object
        /// belongs to.
        void registerScript();

        /// @brief Detach script from the scene it is registered at.
        void detachScript();

    public:
        /// @param script Pointer to the input processing script which the 
        /// component will use.
        InputProcessingScriptComponent(InputProcessingScriptPtr script);

        virtual ~InputProcessingScriptComponent();

        /// @brief Get a pointer to the script used by the component.
        ///
        /// @return Pointer to the script used by the component.
        InputProcessingScriptPtr getScript();

        /// @brief Set the script used by the component.
        ///
        /// @param script Pointer to the script which the component will use.
        void setScript(InputProcessingScriptPtr script);

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
        virtual InputProcessingScriptComponent* clone();
};

template<>
ComponentType Component::componentType<InputProcessingScriptComponent>();

template<>
std::string Component::componentTypeString<InputProcessingScriptComponent>();

#endif//CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP