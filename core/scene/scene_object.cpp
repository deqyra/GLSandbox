#include "scene_object.hpp"

#include <stdexcept>

#include "scene.hpp"

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject() :
    id(_count++),
    enabled(true),
    _scene(std::shared_ptr<Scene>(nullptr)),
    _components(),
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

SceneWPtr SceneObject::getScene()
{
    return _scene;
}

void SceneObject::setScene(ScenePtr wScene)
{
    _scene = wScene;
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

std::vector<ComponentWPtr> SceneObject::getAllComponents()
{
    std::vector<ComponentWPtr> components;
    components.reserve(_components.size());

    // During this copy, all ComponentPtrs are copied as ComponentsWPtrs)
    std::copy(_components.begin(), _components.end(), std::back_inserter(components));

    return components;
}
