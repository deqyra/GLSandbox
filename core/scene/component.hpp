#ifndef CORE__SCENE__COMPONENT_HPP
#define CORE__SCENE__COMPONENT_HPP

#include "component_type.hpp"

#include <memory>
#include <string>

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

/*         ╔════════════╗
 *         ║   README   ║
 *         ╚════════════╝
 * 
 *     DERIVING FROM Component
 * ===============================
 *
 * - Write new component
 * - Add enum value to ComponentType
 * - Override the following methods like so:
 *     template<>
 *     ComponentType Component::componentType<MyNewComponent>()
 *     {
 *        return ComponentType::MyNewComponent;
 *     }
 *
 *     template<>
 *     std::string Component::componentTypeString<MyNewComponent>()
 *     {
 *         return "MyNewComponent";
 *     }
 */

/// @brief Abstract class meant to represent a certain aspect in a scene object
/// (a mesh, a light, etc). Refer to README section at the top of the .hpp file
/// for info about deriving from Component.
class Component
{
    private:
        Component() = delete;
        Component(const Component& other) = delete;
        Component& operator=(const Component& other) = delete;

    protected:
        /// @param type Literal describing the concrete type of this component.
        /// @param sceneObject Pointer to the SceneObject instance this 
        /// component belongs to.
        ///
        /// @exception If the passed script component type is Unknown, the 
        /// function will throw a std::runtime_error.
        Component(ComponentType type, SceneObjectWPtr sceneObject = SceneObjectWPtr());

        /// @brief Pointer to the SceneObject this component belongs to.
        SceneObjectWPtr _sceneObject;

    public:
        virtual ~Component();

        /// @brief Get a pointer to the parent scene object of this component.
        ///
        /// @return A pointer to the parent scene object of this component.
        virtual SceneObjectWPtr getSceneObject();

        /// @brief Set the parent scene object to this component..
        ///
        /// @param sceneObject A pointer to the new parent scene object of this
        /// component.
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual Component* clone() = 0;

        /// @brief Literal describing the concrete type of this component.
        const ComponentType type;

        /// @brief Get a literal describing the concrete type of a Component 
        /// subclass.
        ///
        /// @tparam T Concrete component subclass whose describing literal is
        /// to be returned.
        ///
        /// @return The literal describing the concrete component class provided
        /// as the template parameter.
        template<class T>
        static ComponentType componentType();

        /// @brief Get a descriptive string of the concrete type of a Component 
        /// subclass.
        ///
        /// @tparam T Concrete component subclass whose describing string is
        /// to be returned.
        ///
        /// @return A descriptive string of the concrete component class provided
        /// as the template parameter.
        template<class T>
        static std::string componentTypeString();
};

using ComponentPtr = std::shared_ptr<Component>;
using ComponentWPtr = std::weak_ptr<Component>;

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

#endif//CORE__SCENE__COMPONENT_HPP