#include "script.hpp"

Script::Script()
{

}

virtual Script::~Script()
{

}

void Script::setSceneObject(SceneObjectWPtr sceneObject)
{
    _sceneObject = sceneObject;
}
