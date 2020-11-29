#include "script.hpp"

unsigned int Script::_count = 0;

Script::Script() :
    id(_count++)
{

}

Script::~Script()
{

}

SceneObjectPtr Script::getSceneObject()
{
    return _sceneObject;
}

void Script::setSceneObject(SceneObjectPtr sceneObject)
{
    _sceneObject = sceneObject;
}
