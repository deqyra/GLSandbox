#include "lighting_sandbox.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/window_factory.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/lights/point_light.hpp>
#include <renderboi/core/frame_of_reference.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

using Ref = FrameOfReference;

#include <renderboi/toolbox/common_macros.hpp>
#include <renderboi/toolbox/factory.hpp>
#include <renderboi/toolbox/input_logger.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/controls/control_scheme_manager.hpp>
#include <renderboi/toolbox/controls/control_event_translator.hpp>
#include <renderboi/toolbox/controls/controlled_entity_manager.hpp>
#include <renderboi/toolbox/mesh_generators/mesh_type.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/scene_renderer.hpp>
#include <renderboi/toolbox/scene/scene_object.hpp>
#include <renderboi/toolbox/scene/component_type.hpp>
#include <renderboi/toolbox/scene/components/all_components.hpp>
#include <renderboi/toolbox/runnables/mouse_camera_manager.hpp>
#include <renderboi/toolbox/runnables/keyboard_movement_script.hpp>
#include <renderboi/toolbox/runnables/basic_window_manager.hpp>
#include <renderboi/toolbox/runnables/camera_aspect_ratio_manager.hpp>

#include <cpptools/enum_map.hpp>

void LightingSandbox::run(GLWindowPtr window)
{
    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Lighting");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);
    
    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderInfo::ShaderFeature::VertexMVP);
    lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentMeshMaterial);
    lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentBlinnPhong);
    ShaderProgram lightingShader = ShaderBuilder::buildShaderProgramFromConfig(lightConfig);

    ScenePtr scene = Factory::makeScene();

    // BIG TORUS
    SceneObjectPtr bigTorusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Big torus", {2.f, 0.5f, 72, 48}, Materials::Emerald, lightingShader);

    // SMALL TORUS
    SceneObjectPtr smallTorusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Small torus", {0.75f, 0.25f, 64, 32}, Materials::Gold, lightingShader);
    
    // AXES
    SceneObjectPtr axesObj = Factory::makeSceneObjectWithMesh<MeshType::Axes>("Axes", {3.f});
    
    // CUBE
    SceneObjectPtr cubeObj = Factory::makeSceneObjectWithMesh<MeshType::Cube>("Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Materials::Default, lightingShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(LightBaseRange);
    cubeObj->addComponent<LightComponent>(light);

    // TETRAHEDRON
    SceneObjectPtr tetrahedronObj = Factory::makeSceneObjectWithMesh<MeshType::Tetrahedron>("Tetrahedron", {0.5f});

    // CAMERA
    SceneObjectPtr cameraObj = Factory::makeSceneObject("Camera");
    CameraPtr camera = std::make_shared<Camera>(CameraParams);
    cameraObj->addComponent<CameraComponent>(camera);

    // Register everything in scene and create relationships
    scene->registerObject(bigTorusObj);
    scene->registerObject(smallTorusObj, bigTorusObj->id);
    scene->registerObject(axesObj);
    scene->registerObject(cubeObj);
    scene->registerObject(tetrahedronObj, smallTorusObj->id);
    scene->registerObject(cameraObj);

    // Link camera to MouseCameraManager
    std::shared_ptr<MouseCameraManager> cameraManager = std::make_shared<MouseCameraManager>(camera);

    // Link camera to CameraAspectRatioManager
    std::shared_ptr<CameraAspectRatioManager> cameraAspectRatioManager = std::make_shared<CameraAspectRatioManager>(camera);

    // Attach object movement script to scene
    std::shared_ptr<LightingSandboxScript> rotationScript = std::make_shared<LightingSandboxScript>(cubeObj, bigTorusObj, smallTorusObj, tetrahedronObj, cameraObj, light, LightBaseRange);
    scene->registerScript(std::static_pointer_cast<Script>(rotationScript));
    
    // Add script component to camera: KeyboardMovementScript
    ControlledEntityManager<KeyboardMovementScript> keyboardScriptManager(std::static_pointer_cast<BasisProvider>(camera));
    cameraObj->addComponent<ScriptComponent>(std::static_pointer_cast<Script>(keyboardScriptManager.getEntity()));

    // Window script
    ControlledEntityManager<BasicWindowManager> windowManager;

    // Instantiate an input logger
    InputLoggerPtr logger = std::make_shared<InputLogger>();

    // Register the scene and the control translator to the splitter
    InputSplitterPtr splitter = std::make_shared<InputSplitter>();
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(logger));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(cameraManager));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(cameraAspectRatioManager));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(rotationScript));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(keyboardScriptManager.getEventTranslator()));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(windowManager.getEntity()));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(windowManager.getEventTranslator()));
    
    // Register the splitter to the window
    window->registerInputProcessor(std::static_pointer_cast<InputProcessor>(splitter));


    // Register the splitter to the window
    window->registerInputProcessor(std::static_pointer_cast<InputProcessor>(splitter));

    const glm::vec3 X = Transform::X;
    const glm::vec3 Y = Transform::Y;
    const glm::vec3 Z = Transform::Z;

    // Move stuff around
    bigTorusObj->transform.rotateBy<Ref::World>((float)glm::radians(90.f), X);
    smallTorusObj->transform.rotateBy<Ref::Parent>(glm::radians(90.f), X);
    smallTorusObj->transform.translateBy<Ref::Parent>(-2.f * X);
    cubeObj->transform.setPosition<Ref::World>(StartingLightPosition);
    tetrahedronObj->transform.translateBy<Ref::Parent>(-1.2f * X);
    tetrahedronObj->transform.rotateBy<Ref::Parent>(glm::radians(90.f), Z);
    cameraObj->transform.setPosition<Ref::World>(StartingCameraPosition);
    cameraObj->transform.rotateBy<Ref::Parent>(glm::radians(180.f), Y);

    SceneRenderer sceneRenderer;

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!window->shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and draw scene
        scene->triggerUpdate();

        Transform t = scene->getWorldTransform(smallTorusObj->id);

        //scene->updateAllTransforms();
        sceneRenderer.renderScene(scene);

        // Refresh screen and process input
        window->swapBuffers();
        window->pollEvents();
    }
    window->setShouldClose(false);

    Factory::destroyScene(scene);

    // Reset everything back to how it was
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::NormalCursor);
    window->detachInputProcessor();
    window->setTitle(title);
}

LightingSandboxScript::LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr bigTorusObj, SceneObjectPtr smallTorusObj, SceneObjectPtr tetrahedronObj, SceneObjectPtr cameraObj, std::shared_ptr<PointLight> light, float baseLightRange) :
    _cubeObj(cubeObj),
    _bigTorusObj(bigTorusObj),
    _smallTorusObj(smallTorusObj),
    _tetrahedronObj(tetrahedronObj),
    _cameraObj(cameraObj),
    _light(light),
    _autoRotate(true),
    _speedFactor(1.75f),
    _sine(LightVariationFrequency),
    _baseRange(baseLightRange)
{
    _sine.start();
}

void LightingSandboxScript::update(float timeElapsed)
{
    _light->setRange(_baseRange + _sine.get() * (LightVariationAmplitude / 2.f));

    if (_autoRotate)
    {
        // Update object transforms
        float delta = _speedFactor * timeElapsed;

        _cubeObj->transform.orbit<Ref::World>((float)glm::radians(45.f * delta), CubeOrbitAxis, glm::vec3(0.f, 3.f, 0.f), true);
        _bigTorusObj->transform.rotateBy<Ref::Parent>((float)glm::radians(45.f * delta), BigTorusRotationAxis);
        _smallTorusObj->transform.orbit<Ref::Parent>((float)glm::radians(45.f * delta), SmallTorusRotationAxis, glm::vec3(0.f, 0.f, 0.f), true);
        _tetrahedronObj->transform.rotateBy<Ref::Self>((float)glm::radians(45.f * delta), TetrahedronRotationAxis);
        _tetrahedronObj->transform.orbit<Ref::Parent>((float)glm::radians(45.f * delta), TetrahedronOrbitAxis, glm::vec3(0.f), true);
    }
    else
    {
        _bigTorusObj->transform.lookAt<Ref::World>(_cameraObj->getWorldTransform().getPosition(), {0.f, 1.f, 0.f});
    }
}

void LightingSandboxScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    if (_autoRotate)
    {
        if (key == Key::Up && (action == Action::Press) && (mods & Mod::Control) && _speedFactor < 10.f)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == Key::Down && (action == Action::Press) && (mods & Mod::Control) && _speedFactor > 0.2f)
        {
            _speedFactor /= 1.1f;
        }
    }

    if (key == Key::Enter && action == Action::Press)
    {
        _autoRotate = !_autoRotate;
    }
}

LightingSandboxScript* LightingSandboxScript::clone()
{
    return nullptr;
}