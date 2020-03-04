#include "script.hpp"

Script::_count = 0;

Script::Script() :
    id(_count++)
{

}

virtual Script::~Script()
{

}

void Script::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
}
