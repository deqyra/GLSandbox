#ifndef CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../script.hpp"

#include <string>

class ScriptComponent : public Component
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Components are meant to be used only through pointers
        ScriptComponent(ScriptComponent& other) = delete;
        ScriptComponent& operator=(const ScriptComponent& other) = delete;

        // Pointer to the actual script resource
        ScriptPtr _script;
        // Register script to the scene which the parent scene object is part of
        void registerScript();
        // Detach script from the scene
        void detachScript();

    public:
        ScriptComponent(ScriptPtr script);
        virtual ~ScriptComponent();

        // Get a pointer to the Script of the component
        ScriptPtr getScript();
        // Set the Script pointer of the component
        void setScript(ScriptPtr Script);

        // Set reference to parent scene object
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        // Get a raw pointer to a new ScriptComponent instance cloned from this. The inner script is cloned as well. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual ScriptComponent* clone();
};

template<>
ComponentType Component::componentType<ScriptComponent>();

template<>
std::string Component::componentTypeString<ScriptComponent>();

#endif//CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP