#include "scene_object.hpp"

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject() :
    id(_count++),
    _components()
{

}
