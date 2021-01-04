#include "component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_object.hpp"
#include "component_type.hpp"

namespace Renderboi
{

Component::Component(const ComponentType type, const SceneObjectPtr sceneObject) :
    type(type),
    _sceneObject(sceneObject)
{
    if (type == ComponentType::Unknown)
    {
        throw std::runtime_error("Component: cannot create a component with type Unknown.");
    }

    if (!sceneObject)
    {
        throw std::runtime_error("Component: cannot create a component from a null scene object pointer.");
    }
}

Component::~Component()
{
    _releaseSceneObject();
}

SceneObjectPtr Component::getSceneObject() const
{
    return _sceneObject;
}

void Component::_releaseSceneObject()
{
    _sceneObject.reset();
}

}//namespace Renderboi
