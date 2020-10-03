#ifndef CORE__SCENE__SCENE_OBJECT_HPP
#define CORE__SCENE__SCENE_OBJECT_HPP

#include "../transform.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <string>
#include <exception>

#include "../object_transform.hpp"

#include "component.hpp"
#include "component_type.hpp"

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

// An object meant to be part of a scene. Abstract entity made up of components that give it concrete aspects in the context of a scene.
class SceneObject : public std::enable_shared_from_this<SceneObject>
{
    private:
        // Copy constructor and copy-assignment operator disallowed because SceneObjects are meant to be used through pointers only. Use clone() instead.
        // Also, calls to weak_from_this(), which are prohibited during construction, would have to be made from within the copy constructor.
        SceneObject(const SceneObject& other) = delete;
        SceneObject& operator=(const SceneObject& other) = delete;

        // Keeps track of how many SceneObjects were created (used as an ID system)
        static unsigned int _count;

        // Components making up this scene object
        std::vector<ComponentPtr> _components;
        // The parent scene of this object
        SceneWPtr _scene;

    public:
        SceneObject();
        // To be called before any other operation is performed on the scene object
        void init();

        // Get the world position of the object in the scene (use transform.getPosition() for local position)
        Transform getWorldTransform();
        // Get the parent scene of this object
        SceneWPtr getScene();
        // Set the parent scene of this object
        void setScene(ScenePtr wScene);

        // Get a pointer to a new SceneObject instance cloned from this. All components are cloned as well. The cloned SceneObject 
        SceneObjectPtr clone();

        // Add a component of type T to this object
        template<class T, class... ArgTypes>
        std::shared_ptr<T> addComponent(ArgTypes&&... args);

        // Whether this object has a component of type T
        template<class T>
        bool hasComponent();

        // Get a pointer to the component of type T on this object (if any)
        template<class T>
        std::shared_ptr<T> getComponent();

        // Get pointers to all components this object is made of
        std::vector<ComponentWPtr> getAllComponents();

        // Unique ID of the object
        const unsigned int id;
        // Whether this object is enabled in the scene
        bool enabled;
        // The 3D transform of this object
        ObjectTransform transform;
};

template<class T, class... ArgTypes>
std::shared_ptr<T> SceneObject::addComponent(ArgTypes&& ... args)
{
    // Disallow more than one component of the same type per object
    if (hasComponent<T>())
    {
        std::string s = "SceneObject: object with ID " + std::to_string(id) + " already has a component of type " + Component::componentTypeString<T>() + " and cannot have another one.";
        throw std::runtime_error(s.c_str());
    }

    // Construct component from passed arguments
    std::shared_ptr<T> realComponent = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    realComponent->setSceneObject(this->weak_from_this());

    // std::shared_ptr<Component> and std::shared_ptr<MyComponent> are not covariant types, even if Component and MyComponent are.
    // Cast to base Component in order to add to collection.
    ComponentPtr baseComponent = std::static_pointer_cast<Component>(realComponent);
    _components.push_back(baseComponent);

    return realComponent;
}

template<class T>
bool SceneObject::hasComponent()
{
    ComponentType expectedType = Component::componentType<T>();
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
    ComponentType expectedType = Component::componentType<T>();
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        if ((*it)->type == expectedType)
        {
            // Given that there's at most one component of each type in a single SceneObject,
            // return the first matching component as soon as it is found.
            return std::static_pointer_cast<T>(*it);
        }
    }

    return std::shared_ptr<T>(nullptr);
}

#endif//CORE__SCENE__SCENE_OBJECT_HPP