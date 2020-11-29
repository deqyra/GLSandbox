#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP

#include <string>

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../input_processing_script.hpp"


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

        /// @brief Release the parent scene object pointer.
        virtual void releaseSceneObject();

    public:
        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param script Pointer to the input processing script which the 
        /// component will use.
        ///
        /// @exception If the provided scene object pointer is null, the 
        /// constructor will throw a std::runtime_error.
        InputProcessingScriptComponent(SceneObjectPtr sceneObject, InputProcessingScriptPtr script);

        virtual ~InputProcessingScriptComponent();

        /// @brief Get a pointer to the script used by the component.
        ///
        /// @return Pointer to the script used by the component.
        InputProcessingScriptPtr getScript();

        /// @brief Set the script used by the component.
        ///
        /// @param script Pointer to the script which the component will use.
        ///
        /// @exception If the passed script pointer is null, the function will
        /// throw a std::runtime_error.
        void setScript(InputProcessingScriptPtr script);

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @param newParent Pointer the scene object which will be parent to
        /// the cloned component instance.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual InputProcessingScriptComponent* clone(SceneObjectPtr newParent);
};

template<>
ComponentType Component::componentType<InputProcessingScriptComponent>();

template<>
std::string Component::componentTypeString<InputProcessingScriptComponent>();

template<>
bool Component::multipleInstancesAllowed<InputProcessingScriptComponent>();

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP