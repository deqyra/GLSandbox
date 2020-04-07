#ifndef CORE__SCENE__COMPONENT_HPP
#define CORE__SCENE__COMPONENT_HPP

#include "../../tools/clonable.hpp"

#include "component_type.hpp"

#include <memory>
#include <string>

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

class Component;
using ComponentPtr = std::shared_ptr<Component>;
using ComponentWPtr = std::weak_ptr<Component>;

class Component : public Clonable<Component>
{
    private:
        Component() = delete;

    protected:
        Component(ComponentType type, SceneObjectWPtr sceneObject = SceneObjectWPtr());
        SceneObjectWPtr _sceneObject;

    public:
        virtual ~Component();

        virtual SceneObjectWPtr getSceneObject();
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        virtual ComponentPtr clone() = 0;

        template<class T>
        static ComponentType componentType();

        template<class T>
        static std::string componentTypeString();

        const ComponentType type;
};

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

/* INHERITING FROM Component
 * ====================================
 * (waiting for a better solution)
 *
 * - Write new component
 * - Add enum value to ComponentType
 * - Include the following functions in the header :
 *     template<>
 *     ComponentType Component::componentType<NewComponent>();
 *
 *     template<>
 *     std::string Component::componentTypeString<NewComponent>();
 * - Implement them in the source file
 */

#endif//CORE__SCENE__COMPONENT_HPP