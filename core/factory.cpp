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

void Factory::addMouseKBScriptsToCameraObject(SceneObjectPtr object)
{
    CameraPtr camera = object->getComponent<CameraComponent>()->getCamera();

    // Add script component to object: MouseCameraScript
    std::shared_ptr<MouseCameraScript> mouseScript = std::make_shared<MouseCameraScript>();
    std::shared_ptr<InputProcessingScript> baseMouseScript = std::static_pointer_cast<InputProcessingScript>(mouseScript);
    object->addComponent<InputProcessingScriptComponent>(baseMouseScript);

    // Add script component to object: KeyboardMovementScript
    std::shared_ptr<BasisProvider> cameraAsBasisProvider = std::static_pointer_cast<BasisProvider>(camera);
    std::shared_ptr<KeyboardMovementScript> kbScript = std::make_shared<KeyboardMovementScript>(cameraAsBasisProvider);
    std::shared_ptr<InputProcessingScript> baseKbScript = std::static_pointer_cast<InputProcessingScript>(kbScript);
    object->addComponent<InputProcessingScriptComponent>(baseKbScript);
}