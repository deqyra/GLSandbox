#include "factory.hpp"

#include <memory>

#include "scene/scene.hpp"
#include "scene/scene_object.hpp"


ScenePtr Factory::MakeScene()
{
    ScenePtr scene = std::make_shared<Scene>();
    scene->_init();
    return scene;
}

void Factory::DestroyScene(ScenePtr scene)
{
    scene->_terminate();
    scene.reset();
}

SceneObjectPtr Factory::MakeSceneObject(std::string name)
{
    SceneObjectPtr sceneObject = std::make_shared<SceneObject>(name);
    sceneObject->init();
    return sceneObject;
}
