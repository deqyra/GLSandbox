#ifndef CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../script.hpp"

#include <string>

class ScriptComponent : public Component
{
    private:
        // Pointer to the actual script resource
        ScriptPtr _script;
        // Register script to the scene which the parent scene object is part of
        void registerScript();
        // Detach script from the scene
        void removeScript();

    public:
        ScriptComponent(ScriptPtr script);
        virtual ~ScriptComponent();
};

template<>
ComponentType Component::componentType<ScriptComponent>();

template<>
std::string Component::componentTypeString<ScriptComponent>();

#endif//CORE__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP