#include "factory.hpp"

#include <memory>

#include "scene/scene.hpp"
#include "scene/scene_object.hpp"

ScenePtr Factory::makeScene()
{
    ScenePtr scene = std::make_shared<Scene>();
    scene->init();
    return scene;
}

SceneObjectPtr Factory::makeSceneObject()
{
    SceneObjectPtr sceneObject = std::make_shared<SceneObject>();
    sceneObject->init();
    return sceneObject;
}
