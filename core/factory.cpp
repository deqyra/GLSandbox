#include "factory.hpp"

#include <memory>

#include "scene/scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/components/input_processing_script_component.hpp"

#include "interfaces/basis_provider.hpp"

#include "scripts/mouse_camera_script.hpp"
#include "scripts/keyboard_movement_script.hpp"

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
