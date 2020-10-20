#include "scene_object.hpp"

#include <stdexcept>

#include "scene.hpp"

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

void SceneObject::init()
{
    transform.setSceneObject(shared_from_this());
}

Transform SceneObject::getWorldTransform()
{
    std::shared_ptr<Scene> scene = _scene.lock();
    return scene->getWorldTransform(id);
}

ScenePtr SceneObject::getScene()
{
    return _scene.lock();
}

void SceneObject::setScene(ScenePtr scene)
{
    _scene = scene;
}

SceneObjectPtr SceneObject::clone()
{
    SceneObjectPtr clonedObject = std::make_shared<SceneObject>();
    clonedObject->enabled = enabled;
    clonedObject->transform = transform;
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        ComponentPtr newComponent = ComponentPtr((*it)->clone());
        newComponent->setSceneObject(clonedObject);
        clonedObject->_components.push_back(newComponent);
    }

    return clonedObject;
}

std::vector<ComponentPtr> SceneObject::getAllComponents()
{
    std::vector<ComponentPtr> components;
    components.reserve(_components.size());

    // During this copy, all ComponentPtrs are copied as ComponentsWPtrs)
    std::copy(_components.begin(), _components.end(), std::back_inserter(components));

    return components;
}
