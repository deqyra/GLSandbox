#ifndef SCENE_TOOLS_HPP
#define SCENE_TOOLS_HPP

#include <memory>

#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"

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

std::shared_ptr<SceneObjectComponent> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr);

template<SceneObjectComponentType T>
std::shared_ptr<typename TypeFromEnum<T>::type> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr);

#endif//SCENE_TOOLS_HPP
