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

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

class Scene;

class SceneObject : public PositionedObject, public std::enable_shared_from_this<SceneObject>
{
    using CompType = SceneObjectComponentType;
    using CompPtr  = std::shared_ptr<SceneObjectComponent>;
    using WeakCompPtr  = std::weak_ptr<SceneObjectComponent>;

    private:
        static unsigned int _count;

        std::vector<CompPtr> _components;

    public:
        SceneObject(std::shared_ptr<Scene> scene);
        SceneObject(const SceneObject& other);
        SceneObject& operator=(const SceneObject& other);

        template<class T, class... ArgTypes>
        std::weak_ptr<T> addComponent(ArgTypes&&... args);

        template<class T>
        bool hasComponent();

        template<class T>
        std::weak_ptr<T> getComponent();

        glm::vec3 getWorldPosition();

        const unsigned int id;
        bool enabled;
        const std::weak_ptr<Scene> scene;
};

template<class T, class... ArgTypes>
std::weak_ptr<T> SceneObject::addComponent(ArgTypes&& ... args)
{
    if (hasComponent<T>())
    {
        std::string s = "SceneObject: object with ID " + std::to_string(id) + " already has a component of type " + SceneObjectComponent::componentTypeString<T>() + " and cannot have another.";
        throw std::runtime_error(s.c_str());
    }

    std::shared_ptr<T> realComp = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    std::shared_ptr<SceneObjectComponent> baseComp = std::static_pointer_cast<SceneObjectComponent>(realComp);
    _components.push_back(baseComp);
    baseComp->sceneObject = this->weak_from_this();
    return realComp;
}

template<class T>
bool SceneObject::hasComponent()
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
std::weak_ptr<T> SceneObject::getComponent()
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