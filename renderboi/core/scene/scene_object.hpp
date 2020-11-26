#ifndef CORE__SCENE__SCENE_OBJECT_HPP
#define CORE__SCENE__SCENE_OBJECT_HPP

#include "../transform.hpp"

#include <exception>
#include <functional>
#include <vector>
#include <memory>
#include <type_traits>
#include <string>

#include "../object_transform.hpp"

#include "component.hpp"
#include "component_type.hpp"

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

/// @brief An object meant to be part of a scene. Abstract entity made up of 
/// components which give it concrete aspects in the context of a scene.
class SceneObject : public std::enable_shared_from_this<SceneObject>
{
    private:
        SceneObject(const SceneObject& other) = delete;
        SceneObject& operator=(const SceneObject& other) = delete;

        /// @brief Keeps track of how many scene objects were instantiated (used
        /// as a unique ID system).
        static unsigned int _count;

        /// @brief Name of the object in the scene.
        std::string _name;

        /// @brief Components making up this scene object.
        std::vector<ComponentPtr> _components;

        /// @brief Pointer to the parent scene of this object.
        ScenePtr _scene;

        friend Scene;

        /// @brief Set the parent scene of this object.
        ///
        /// @param scene A pointer to the new parent scene of this object.0
        void setScene(ScenePtr scene);

    public:
        /// @param name Name to give to the scene object.
        SceneObject(std::string name = "");

        ~SceneObject();

        /// @brief Link the scene object to its transform (which cannot be done
        /// during construction). To be called before any other operation is 
        /// performed on the scene object.
        void init();

        /// @brief Get the world transform of the object in the scene.
        ///
        /// @return The world transform of the object in the scene.
        Transform getWorldTransform();

        /// @brief Get the parent scene of this object.
        ///
        /// @return A pointer to the parent scene of this object.
        ScenePtr getScene();

        /// @brief Get a raw pointer to a new scene object instance cloned from
        /// this one. All components are cloned as well. Ownership and 
        /// responsibility for the allocated resources are fully transferred to
        /// the caller.
        ///
        /// @return A raw pointer to the cloned scene object instance.
        SceneObjectPtr clone();

        /// @brief Construct a component and add it to this object.
        ///
        /// @tparam T Concrete type of the component to construct.
        /// @tparam ArgTypes... Types of the arguments to pass to the concrete
        /// component constructor. Will be automatically deduced.
        ///
        /// @param args Arguments to pass to the concrete component constructor.
        ///
        /// @return A pointer to the constructed component.
        template<class T, class... ArgTypes>
        std::shared_ptr<T> addComponent(ArgTypes&&... args);

        /// @brief Whether this object has a certain component.
        ///
        /// @tparam Type of the concrete component to test for.
        /// 
        /// @return Whether or not the object has a component of specified type.
        template<class T>
        bool hasComponent();

        /// @brief Get pointer to the first component of a certain type on this 
        /// object.
        /// 
        /// @tparam Type of the concrete component to test for.
        /// 
        /// @return A pointer to the first component of this object which fits 
        /// the criteria, or nullptr if the requested component is not present.
        template<class T>
        std::shared_ptr<T> getComponent();

        /// @brief Get an array of pointers to components of a certain type on
        /// this object.
        /// 
        /// @tparam Type of the concrete component to test for.
        /// 
        /// @return An array of pointers to components of a certain type on this
        /// object.
        template<class T>
        std::vector<std::shared_ptr<T>> getComponents();

        /// @brief Get pointers to all the components this object is made of.
        ///
        /// @return An array of pointers to all the components this object is 
        /// made of.
        std::vector<ComponentPtr> getAllComponents();

        /// @brief Unique ID of the object.
        const unsigned int id;

        /// @brief Whether this object is enabled in its parent scene.
        bool enabled;

        /// @brief The 3D transform of this object.
        ObjectTransform transform;
};

template<class T, class... ArgTypes>
std::shared_ptr<T> SceneObject::addComponent(ArgTypes&& ... args)
{
    // Disallow more than one component of the same type per object,
    // unless specifically allowed by component type
    if (!Component::multipleInstancesAllowed<T>() && hasComponent<T>())
    {
        std::string s = "SceneObject: object with ID " + std::to_string(id) + " already has a component of type " + Component::componentTypeString<T>() + " and cannot have another one.";
        throw std::runtime_error(s.c_str());
    }

    // Construct component from passed arguments
    std::shared_ptr<T> realComponent = std::make_shared<T>(shared_from_this(), std::forward<ArgTypes>(args)...);

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
            return std::static_pointer_cast<T>(*it);
        }
    }

    return nullptr;
}

template<class T>
std::vector<std::shared_ptr<T>> SceneObject::getComponents()
{
    ComponentType expectedType = Component::componentType<T>();
    std::vector<std::shared_ptr<T>> components;

    std::function<bool(std::shared_ptr<T>)> checkType = [expectedType](std::shared_ptr<T> val) -> bool
    {
        return val->type == expectedType;
    };

    std::copy_if(_components.begin(), _components.end(), std::back_inserter(components), checkType);

    return components;
}

#endif//CORE__SCENE__SCENE_OBJECT_HPP