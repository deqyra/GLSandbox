#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENT_HPP

#include <memory>
#include <string>

#include "component_type.hpp"

namespace Renderboi
{

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
 * - Specialize the following static methods from Component like so:
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
 * 
 *     template<>
 *     bool Component::multipleInstancesAllowed<MyNewComponent>()
 *     {
 *         return (true or false);
 *     }
 */

/// @brief Abstract class meant to represent a certain aspect in a scene object
/// (a mesh, a light, etc). Refer to README section at the top of the .hpp file
/// for info about deriving from Component.
class Component
{
private:
    friend SceneObject;

    Component() = delete;
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;

protected:
    /// @param type Literal describing the concrete type of this component.
    /// @param sceneObject Pointer to the SceneObject instance this 
    /// component belongs to.
    ///
    /// @exception If the passed component type is Unknown, or if the 
    /// provided scene object pointer is null, the constructor will throw a
    /// std::runtime_error.
    Component(const ComponentType type, const SceneObjectPtr sceneObject);

    /// @brief Pointer to the SceneObject this component belongs to.
    SceneObjectPtr _sceneObject;

    /// @brief Release the parent scene object pointer.
    virtual void _releaseSceneObject();

public:
    virtual ~Component();

    /// @brief Get a pointer to the parent scene object of this component.
    ///
    /// @return A pointer to the parent scene object of this component.
    SceneObjectPtr getSceneObject() const;

    /// @brief Get a raw pointer to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw pointer to the component instance cloned from this 
    /// one.
    virtual Component* clone(const SceneObjectPtr newParent) const = 0;

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
    /// @return A descriptive string of the concrete component class 
    /// provided as the template parameter.
    template<class T>
    static std::string componentTypeString();

    /// @brief Returns whether more than one instance of this component can
    /// be present on a same scene object.
    ///
    /// @tparam T Concrete component subclass to test.
    ///
    /// @return Whether or not more than one instance of this component can
    /// be present on a same scene object.
    template<class T>
    static bool multipleInstancesAllowed();
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

template<class T>
bool Component::multipleInstancesAllowed()
{
    return false;
}

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENT_HPP