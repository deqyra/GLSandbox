#include "component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_object.hpp"
#include "component_type.hpp"

Component::Component(ComponentType type, SceneObjectWPtr sceneObject) :
    type(type),
    _sceneObject(sceneObject)
{
    if (type == ComponentType::Unknown)
    {
        throw std::runtime_error("Component: cannot create a component with type Unknown.");
    }
}

Component::~Component()
{

}

SceneObjectWPtr Component::getSceneObject()
{
    return _sceneObject;
}

void Component::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
}
