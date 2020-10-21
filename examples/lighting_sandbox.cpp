#include "lighting_sandbox.hpp"

#include <memory>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../window/gl_window.hpp"
#include "../window/tools.hpp"

#include "../core/common_macros.hpp"
#include "../core/mesh.hpp"
#include "../core/materials.hpp"
#include "../core/shader.hpp"
#include "../core/lights/point_light.hpp"
#include "../core/frame_of_reference.hpp"

using Ref = FrameOfReference;

#include "../core/factory.hpp"
#include "../core/mesh_generators/mesh_type.hpp"

#include "../core/scene/scene.hpp"
#include "../core/scene/scene_renderer.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/component_type.hpp"
#include "../core/scene/components/all_components.hpp"

#include "../core/scripts/mouse_camera_script.hpp"
#include "../core/scripts/keyboard_movement_script.hpp"
#include "../core/scripts/basic_input_manager.hpp"

LightingSandbox::LightingSandbox()
{
    
}

LightingSandbox::~LightingSandbox()
{

}

void LightingSandbox::run(GLWindowPtr window)
{
    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Lighting");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    ScenePtr scene = Factory::makeScene();

    // Register the scene as an input processor to the window
    window->registerInputProcessor(scene);

    // BIG TORUS
    SceneObjectPtr bigTorusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Big torus", {2.f, 0.5f, 72, 48}, Materials::Emerald, lightingShader);

    // SMALL TORUS
    SceneObjectPtr smallTorusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Small torus", {0.75f, 0.25f, 64, 32}, Materials::Gold, lightingShader);
    
    // AXES
    SceneObjectPtr axesObj = Factory::makeSceneObjectWithMesh<MeshType::Axes>("Axes", {3.f});
    
    // CUBE
    SceneObjectPtr cubeObj = Factory::makeSceneObjectWithMesh<MeshType::Cube>("Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Material(), lightingShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(LightBaseRange);
    cubeObj->addComponent<LightComponent>(light);

    // TETRAHEDRON
    SceneObjectPtr tetrahedronObj = Factory::makeSceneObjectWithMesh<MeshType::Tetrahedron>("Tetrahedron", {0.5f});
    tetrahedronObj->enabled = false;

    // CAMERA
    SceneObjectPtr cameraObj = Factory::makeSceneObject("Camera");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), window->getAspectRatio(), 0.1f, 100.0f);
    CameraPtr camera = std::make_shared<Camera>(projection, CameraYaw, CameraPitch);
    cameraObj->addComponent<CameraComponent>(camera);

    // Register everything in scene and create relationships
    scene->registerObject(bigTorusObj);
    scene->registerObject(smallTorusObj, bigTorusObj->id);
    scene->registerObject(axesObj);
    scene->registerObject(cubeObj);
    scene->registerObject(tetrahedronObj, smallTorusObj->id);
    scene->registerObject(cameraObj);

    // Add scripts components to camera : MouseCameraScript
    std::shared_ptr<MouseCameraScript> mouseScript = std::make_shared<MouseCameraScript>();
    std::shared_ptr<InputProcessingScript> baseMouseScript = std::static_pointer_cast<InputProcessingScript>(mouseScript);
    cameraObj->addComponent<InputProcessingScriptComponent>(baseMouseScript);

    // Add scripts components to camera : KeyboardMovementScript
    std::shared_ptr<BasisProvider> cameraAsBasisProvider = std::static_pointer_cast<BasisProvider>(camera);
    std::shared_ptr<KeyboardMovementScript> kbScript = std::make_shared<KeyboardMovementScript>(cameraAsBasisProvider);
    std::shared_ptr<InputProcessingScript> baseKbScript = std::static_pointer_cast<InputProcessingScript>(kbScript);
    cameraObj->addComponent<InputProcessingScriptComponent>(baseKbScript);

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
    
    // ROTATION SCRIPT
    std::shared_ptr<LightingSandboxScript> rotationScript = std::make_shared<LightingSandboxScript>(cubeObj, bigTorusObj, smallTorusObj, tetrahedronObj, cameraObj, light, LightBaseRange);
    std::shared_ptr<InputProcessingScript> ipRotationScript = std::static_pointer_cast<InputProcessingScript>(rotationScript);
    scene->registerInputProcessingScript(ipRotationScript);
    
    // WINDOW SCRIPT
    std::shared_ptr<BasicInputManager> windowScript = std::make_shared<BasicInputManager>();
    std::shared_ptr<InputProcessingScript> ipWindowScript = std::static_pointer_cast<InputProcessingScript>(windowScript);
    scene->registerInputProcessingScript(ipWindowScript);

    SceneRenderer sceneRenderer;

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!window->shouldClose())
    {
        float frameTime = (float)glfwGetTime();
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
        /*
        // Update object transforms
        float delta = _speedFactor * timeElapsed;

        _cubeObj->transform.orbit<Ref::World>((float)glm::radians(45.f * delta), CubeOrbitAxis, glm::vec3(0.f, 3.f, 0.f), true);
        _bigTorusObj->transform.rotateBy<Ref::Parent>((float)glm::radians(45.f * delta), BigTorusRotationAxis);
        _smallTorusObj->transform.orbit<Ref::Parent>((float)glm::radians(45.f * delta), SmallTorusRotationAxis, glm::vec3(0.f, 0.f, 0.f), true);
        _tetrahedronObj->transform.rotateBy<Ref::Self>((float)glm::radians(45.f * delta), TetrahedronRotationAxis);
        _tetrahedronObj->transform.orbit<Ref::Parent>((float)glm::radians(45.f * delta), TetrahedronOrbitAxis, glm::vec3(0.f), true);
        */
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
        if (key == Key::Up && (action == Action::Press || action == Action::Repeat) && (mods & Mod::Control) && _speedFactor < 10.f)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == Key::Down && (action == Action::Press || action == Action::Repeat) && (mods & Mod::Control) && _speedFactor > 0.2f)
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