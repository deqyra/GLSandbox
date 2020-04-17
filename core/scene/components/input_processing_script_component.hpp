#ifndef CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../input_processing_script.hpp"

#include <string>

class InputProcessingScriptComponent : public Component
{
    private:
        // Pointer to the actual script resource
        InputProcessingScriptPtr _script;
        // Register script to the scene which the parent scene object is part of
        void registerScript();
        // Detach script from the scene
        void detachScript();

    public:
        InputProcessingScriptComponent(InputProcessingScriptPtr script);
        virtual ~InputProcessingScriptComponent();

        // Get a pointer to the Script of the component
        InputProcessingScriptPtr getScript();
        // Set the Script pointer of the component
        void setScript(InputProcessingScriptPtr Script);

        // Set reference to parent scene object
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        // Get a raw pointer to a new InputProcessingScriptComponent instance cloned from this. The inner script is cloned as well. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual InputProcessingScriptComponent* clone();
};

template<>
ComponentType Component::componentType<InputProcessingScriptComponent>();

template<>
std::string Component::componentTypeString<InputProcessingScriptComponent>();

#endif//CORE__SCENE__COMPONENTS__INPUT_PROCESSING_SCRIPT_COMPONENT_HPP