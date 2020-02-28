#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "../positioned_object.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <string>
#include <exception>

#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"

class SceneObject : public PositionedObject
{
    using CompType = SceneObjectComponentType;
    using CompPtr  = std::shared_ptr<SceneObjectComponent>;

    private:
        static unsigned int _count;

        std::vector<CompPtr> _components;

    public:
        SceneObject();

        template<class T, class... ArgTypes>
        std::shared_ptr<T> addComponent(ArgTypes&&... args);

        template<class T>
        std::shared_ptr<T> hasComponent();

        template<class T>
        std::shared_ptr<T> getComponent();

        const unsigned int id;
};

template<class T, class... ArgTypes>
std::shared_ptr<T> SceneObject::addComponent(ArgTypes&& ... args)
{
    if (hasComponent<T>())
    {
        std::string s = "SceneObject: object with ID " + std::to_string(id) + " already has a component of type " + SceneObjectComponent::componentTypeString<T>() + " and cannot have another.";
        throw std::runtime_error(s.c_str());
    }

    std::shared_ptr<T> comp = std::make_shared<T>(std::forward<ArgTypes>(args));
    _components.push_back(comp);
    return comp;
}

template<class T>
std::shared_ptr<T> SceneObject::hasComponent()
{
    SceneObjectComponentType expectedType = SceneObjectComponent::componentType<T>();
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if ((*it)->type == expectedType)
        {
            return true;
        }
    }

    return false;
}

template<class T>
std::shared_ptr<T> SceneObject::getComponent()
{
    SceneObjectComponentType expectedType = SceneObjectComponent::componentType<T>();
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if ((*it)->type == expectedType)
        {
            return std::static_pointer_cast<T>(*it);
        }
    }

    return std::shared_ptr<T>(nullptr);
}

#endif//SCENE_OBJECT_HPP