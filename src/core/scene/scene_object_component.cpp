#include "scene_object_component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_types_decl.hpp"
#include "scene_object_component_type.hpp"

SceneObjectComponent::SceneObjectComponent(SceneObjectComponentType type, SceneObjectWPtr sceneObject) :
    type(type),
    _sceneObject(sceneObject)
{
    if (type == SceneObjectComponentType::Unknown)
    {
        throw std::runtime_error("SceneObjectComponent: cannot create a component with type Unknown.");
    }
}

SceneObjectComponent::~SceneObjectComponent()
{

}

SceneObjectWPtr SceneObjectComponent::getSceneObject()
{
    return _sceneObject;
}

void SceneObjectComponent::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
}
