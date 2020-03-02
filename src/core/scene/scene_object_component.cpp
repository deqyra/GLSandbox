#include "scene_object_component.hpp"

#include <stdexcept>
#include <memory>

#include "scene_object_component_type.hpp"

SceneObjectComponent::SceneObjectComponent(SceneObjectComponentType type, ObjPtr sceneObject) :
    type(type),
    sceneObject(sceneObject)
{
    if (type == SceneObjectComponentType::Unknown)
    {
        throw std::runtime_error("SceneObjectComponent: cannot create a component with type Unknown.");
    }
}

SceneObjectComponent::~SceneObjectComponent()
{

}