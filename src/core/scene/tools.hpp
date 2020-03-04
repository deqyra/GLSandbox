#ifndef SCENE_TOOLS_HPP
#define SCENE_TOOLS_HPP

#include <memory>

#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"
#include "scene_types_decl.hpp"

template<SceneObjectComponentType T>
struct TypeFromEnum
{
    using type = void;
};

template<>
struct TypeFromEnum<SceneObjectComponentType::Mesh>
{
    using type = MeshComponent;
};

template<>
struct TypeFromEnum<SceneObjectComponentType::Light>
{
    using type = LightComponent;
};

template<>
struct TypeFromEnum<SceneObjectComponentType::Camera>
{
    using type = CameraComponent;
};

template<>
struct TypeFromEnum<SceneObjectComponentType::Script>
{
    using type = ScriptComponent;
};

std::shared_ptr<SceneObjectComponent> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr);

template<SceneObjectComponentType T>
std::shared_ptr<typename TypeFromEnum<T>::type> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr)
{
    using ComponentRealType = typename TypeFromEnum<T>::type;
    using ComponentPtr = std::shared_ptr<ComponentRealType>;

    ComponentPtr realTypePtr = std::static_pointer_cast<ComponentRealType>(compPtr);
    // Clone the component on the heap using his real type copy constructor
    realTypePtr.reset(new ComponentRealType(*realTypePtr));
    // Return a pointer to it
    return realTypePtr;
}

#endif//SCENE_TOOLS_HPP
