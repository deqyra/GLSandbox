#include "scene_object.hpp"

#include <stdexcept>

#include "scene.hpp"

namespace Renderboi
{

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(std::string name) :
    id(_count++),
    enabled(true),
    _name(name),
    _components(),
    _scene(std::shared_ptr<Scene>(nullptr)),
    transform()
{

}

SceneObject::~SceneObject()
{
    for (const auto& component : _components)
    {
        component->_releaseSceneObject();
    }
}

void SceneObject::init()
{
    transform.setSceneObject(shared_from_this());
}

Transform SceneObject::getWorldTransform() const
{
    return _scene->getWorldTransform(id);
}

ScenePtr SceneObject::getScene() const
{
    return _scene;
}

void SceneObject::setScene(ScenePtr scene)
{
    _scene = scene;
}

SceneObjectPtr SceneObject::clone() const
{
    SceneObjectPtr clonedObject = std::make_shared<SceneObject>();
    clonedObject->enabled = enabled;
    clonedObject->transform = transform;

    for (const auto& component : _components)
    {
        ComponentPtr newComponent = ComponentPtr(component->clone(clonedObject));
        clonedObject->_components.push_back(newComponent);
    }

    return clonedObject;
}

std::vector<ComponentPtr> SceneObject::getAllComponents() const
{
    std::vector<ComponentPtr> components;
    components.reserve(_components.size());

    // During this copy, all ComponentPtrs are copied as ComponentsWPtrs)
    std::copy(_components.begin(), _components.end(), std::back_inserter(components));

    return components;
}

}//namespace Renderboi
