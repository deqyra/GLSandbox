#include "scene_object.hpp"

#include <stdexcept>

#include "../scene.hpp"
#include "tools.hpp"

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(ScenePtr scene) :
    id(_count++),
    enabled(true),
    _scene(scene),
    _components()
{

}

SceneObject::SceneObject(const SceneObject& other) :
    id(_count++),
    enabled(other.enabled),
    _components()
{
    for (auto it = other._components.begin(); it != other._components.end(); it++)
    {
        SceneObjectComponentPtr newComponent = cloneComponent(*it);
        newComponent->setSceneObject(this->weak_from_this());
        _components.push_back(newComponent);
    }
}

SceneObject& SceneObject::operator=(const SceneObject& other)
{
    enabled = other.enabled;
    _components.clear();
    for (auto it = other._components.begin(); it != other._components.end(); it++)
    {
        SceneObjectComponentPtr newComponent = cloneComponent(*it);
        newComponent->setSceneObject(this->weak_from_this());
        _components.push_back(newComponent);
    }

    return (*this);
}

glm::vec3 SceneObject::getWorldPosition()
{
    std::shared_ptr<Scene> scene = _scene.lock();
    return scene->getWorldPosition(id);
}

SceneWPtr SceneObject::getScene()
{
    return _scene;
}

void SceneObject::setScene(SceneWPtr scene)
{
    _scene = scene;
}
