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

void Factory::destroyScene(ScenePtr& scene)
{
    scene->terminate();
    scene.reset();
}

SceneObjectPtr Factory::makeSceneObject(std::string name)
{
    SceneObjectPtr sceneObject = std::make_shared<SceneObject>(name);
    sceneObject->init();
    return sceneObject;
}
