#ifndef CORE__SCENE__COMPONENT_HPP
#define CORE__SCENE__COMPONENT_HPP

#include "component_type.hpp"

#include <memory>
#include <string>

// Empty declaration of class SceneObject, used as a pointer in class Component
class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

// Abstract class meant to represent a certain aspect in a scene object: a mesh, a light, etc.
class Component
{
    private:
        Component() = delete;
        // Disallow copy-constructor and copy-assignment operator as Components are meant to be used only through pointers
        Component(const Component& other) = delete;
        Component& operator=(const Component& other) = delete;

    protected:
        // To be called by all derived Components at construction
        Component(ComponentType type, SceneObjectWPtr sceneObject = SceneObjectWPtr());
        // Reference to the parent scene object
        SceneObjectWPtr _sceneObject;

    public:
        virtual ~Component();

        // Get reference to parent scene object
        virtual SceneObjectWPtr getSceneObject();
        // Set reference to parent scene object
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        // Get a raw pointer to a new Component instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual Component* clone() = 0;

        // Literal representing the type of this component
        const ComponentType type;

        // Get a literal representing the type of a Component subclass (template parameter)
        template<class T>
        static ComponentType componentType();

        // Get a string representing the type of a Component subclass (template parameter)
        template<class T>
        static std::string componentTypeString();
};

using ComponentPtr = std::shared_ptr<Component>;
using ComponentWPtr = std::weak_ptr<Component>;

// For any passed type, componentType and componentTypeString return default values.
// New specialized version of these functions should be written with any new Component subclass,
// in order for a non-default, meaningful literal to be returned for that new subclass.

template<class T>
ComponentType Component::componentType()
{
    return ComponentType::Unknown;
}

template<class T>
std::string Component::componentTypeString()
{
    return "Unknown";
}

/* DERIVING FROM Component
 * ===============================
 * (waiting for a better solution)
 *
 * - Write new component
 * - Add enum value to ComponentType
 * - Declare following functions in header:
 *     template<>
 *     ComponentType Component::componentType<MyNewComponent>();        // return ComponentType::MyNewComponent;
 *
 *     template<>
 *     std::string Component::componentTypeString<MyNewComponent>();    // return "MyNewComponent";
 * - Implement in source file
 */

#endif//CORE__SCENE__COMPONENT_HPP