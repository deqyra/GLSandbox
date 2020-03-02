#include "scene_object.hpp"

#include <stdexcept>

#include "tools.hpp"

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(std::shared_ptr<Scene> scene) :
    id(_count++),
    enabled(true),
    _components(),
    _scene(scene)
{

}

SceneObject::SceneObject(const SceneObject& other) :
    id(_count++),
    enabled(other.enabled),
    _components()
{
    for (auto it = other._components.begin(); it != other._components.end(); it++)
    {
        CompPtr newComp = cloneComponent(*it);
        newComp->sceneObject = this->weak_from_this();
        _components.push_back(newComp);
    }
}

SceneObject& SceneObject::operator=(const SceneObject& other)
{
    enabled = other.enabled;
    _components.clear();
    for (auto it = other._components.begin(); it != other._components.end(); it++)
    {
        CompPtr newComp = cloneComponent(*it);
        newComp->sceneObject = this->weak_from_this();
        _components.push_back(newComp);
    }

    return (*this);
}
