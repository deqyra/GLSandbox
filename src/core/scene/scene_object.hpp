#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "../positioned_object.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <string>
#include <exception>

#include "scene_types_decl.hpp"
#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"
#include "components/script_component.hpp"

class SceneObject : public PositionedObject, public std::enable_shared_from_this<SceneObject>
{
    private:
        static unsigned int _count;

        std::vector<SceneObjectComponentPtr> _components;
        SceneWPtr _scene;

    public:
        SceneObject(SceneWPtr scene);
        SceneObject(const SceneObject& other);
        SceneObject& operator=(const SceneObject& other);

        glm::vec3 getWorldPosition();
        SceneWPtr getScene();
        void setScene(SceneWPtr scene);

        template<class T, class... ArgTypes>
        std::weak_ptr<T> addComponent(ArgTypes&&... args);

        template<class T>
        bool hasComponent();

        template<class T>
        std::weak_ptr<T> getComponent();

        template<class T>
        std::weak_ptr<T> getAllComponents();

        const unsigned int id;
        bool enabled;
};

template<class T, class... ArgTypes>
std::weak_ptr<T> SceneObject::addComponent(ArgTypes&& ... args)
{
    if (hasComponent<T>() && SceneObjectComponent::componentType<T>() != SceneObjectComponentType::Script)
    {
        std::string s = "SceneObject: object with ID " + std::to_string(id) + " already has a component of type " + SceneObjectComponent::componentTypeString<T>() + " and cannot have another.";
        throw std::runtime_error(s.c_str());
    }

    std::shared_ptr<T> realComp = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    realComp->setSceneObject(this->weak_from_this());

    SceneObjectComponentPtr baseComp = std::static_pointer_cast<SceneObjectComponent>(realComp);
    _components.push_back(baseComp);

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

    return std::weak_ptr<T>(nullptr);
}

template<class T>
std::weak_ptr<T> SceneObject::getAllComponents()
{
    std::vector<std::weak_ptr<T>> components;

    SceneObjectComponentType expectedType = SceneObjectComponent::componentType<T>();
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if ((*it)->type == expectedType)
        {
            SceneObjectComponentWPtr wComponent = std::static_pointer_cast<T>(*it);
            std::weak_ptr<T> wRealComponent = std::weak_ptr<T>(wComponent);
            components.push_back(wRealComponent);
        }
    }

    return components;
}

#endif//SCENE_OBJECT_HPP