#include "script.hpp"

unsigned int Script::_count = 0;

Script::Script() :
    id(_count++)
{

}

Script::~Script()
{

}

SceneObjectWPtr Script::getSceneObject()
{
    return _sceneObject;
}

void Script::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
}
