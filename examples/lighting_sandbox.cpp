#include "lighting_sandbox.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../window/gl_window.hpp"
#include "../window/tools.hpp"

#include "../core/gl_macros.hpp"
#include "../core/mesh.hpp"
#include "../core/materials.hpp"
#include "../core/shader.hpp"
#include "../core/lights/point_light.hpp"

#include "../core/mesh_generators/torus_generator.hpp"
#include "../core/mesh_generators/axes_generator.hpp"
#include "../core/mesh_generators/cube_generator.hpp"
#include "../core/mesh_generators/tetrahedron_generator.hpp"

#include "../core/scene/scene.hpp"
#include "../core/scene/scene_renderer.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/component_type.hpp"
#include "../core/scene/components/all_components.hpp"

#include "../core/scripts/fps_camera_script.hpp"
#include "../core/scripts/basic_input_manager.hpp"

LightingSandbox::LightingSandbox()
{
    
}

LightingSandbox::~LightingSandbox()
{

}

void LightingSandbox::run(GLWindowPtr window)
{
    const float lightBaseRange = 50.f;

    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Lighting");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), window->getAspectRatio(), 0.1f, 100.0f);
    CameraPtr _camera = std::make_shared<Camera>(projection, -135.f, -25.f);
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    // Initialize scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->init();

    // Retrieve the custom window pointer, register the scene as an input processor
    window->registerInputProcessor(scene);

    // BIG TORUS
    std::shared_ptr<SceneObject> bigTorusObj = generateSceneMesh(scene, std::make_shared<TorusGenerator>(2.f, 0.5f, 72, 48), Materials::Emerald, lightingShader);
    scene->registerObject(bigTorusObj);

    // SMALL TORUS
    std::shared_ptr<SceneObject> smallTorusObj = generateSceneMesh(scene, std::make_shared<TorusGenerator>(0.75f, 0.25f, 64, 32), Materials::Gold, lightingShader);
    scene->registerObject(smallTorusObj, bigTorusObj->id);
    smallTorusObj->transform.rotate(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    smallTorusObj->transform.translate(glm::vec3(-2.f, 0.f, 0.f));
    
    // AXES
    std::shared_ptr<SceneObject> axesObj = generateSceneMesh(scene, std::make_shared<AxesGenerator>(3.f));
    scene->registerObject(axesObj);
    
    // CUBE
    std::shared_ptr<SceneObject> cubeObj = generateSceneMesh(scene, std::make_shared<CubeGenerator>(), Material(), lightingShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(lightBaseRange);
    cubeObj->addComponent<LightComponent>(light);
    cubeObj->transform.setPosition(glm::vec3(-3.f, 3.f, 0.f));
    scene->registerObject(cubeObj);
    
    // TETRAHEDRON
    std::shared_ptr<SceneObject> tetrahedronObj = generateSceneMesh(scene, std::make_shared<TetrahedronGenerator>(0.5f));
    scene->registerObject(tetrahedronObj, smallTorusObj->id);
    //tetrahedronObj->translate(glm::vec3(1.2f, 0.f, 0.f));
    tetrahedronObj->transform.rotate(glm::radians(90.f), glm::vec3(0.F, 0.f, 1.f));
    
    // CAMERA
    std::shared_ptr<SceneObject> cameraObj = std::make_shared<SceneObject>(scene);
    cameraObj->addComponent<CameraComponent>(_camera);
    cameraObj->transform.setPosition(StartingCameraPosition);
    scene->registerObject(cameraObj);
    std::shared_ptr<FPSCameraScript> fpsScript = std::make_shared<FPSCameraScript>();
    std::shared_ptr<InputProcessingScript> baseFpsScript = std::static_pointer_cast<InputProcessingScript>(fpsScript);
    cameraObj->addComponent<InputProcessingScriptComponent>(baseFpsScript);
    
    // ROTATION SCRIPT
    std::shared_ptr<LightingSandboxScript> rotationScript = std::make_shared<LightingSandboxScript>(cubeObj, bigTorusObj, smallTorusObj, tetrahedronObj, light, lightBaseRange);
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
        sceneRenderer.renderScene(scene);

        // Refresh screen and process input
        window->swapBuffers();
        window->pollEvents();
    }
    window->setShouldClose(false);
    scene->detachInputProcessingScript(ipRotationScript);

    // Reset everything back to how it was
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::NormalCursor);
    window->detachInputProcessor();
    window->setTitle(title);
}

std::shared_ptr<SceneObject> LightingSandbox::generateSceneMesh(std::shared_ptr<Scene> scene, std::shared_ptr<MeshGenerator> generator, Material mat, Shader shader)
{
    MeshPtr mesh = generator->generatePtr();
    std::shared_ptr<SceneObject> obj = std::make_shared<SceneObject>(SceneWPtr(scene));
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}

LightingSandboxScript::LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr bigTorusObj, SceneObjectPtr smallTorusObj, SceneObjectPtr tetrahedronObj, std::shared_ptr<PointLight> light, float baseLightRange) :
    _cubeObj(cubeObj),
    _bigTorusObj(bigTorusObj),
    _smallTorusObj(smallTorusObj),
    _tetrahedronObj(tetrahedronObj),
    _light(light),
    _autoRotate(true),
    _speedFactor(10.f),
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
        float angleDiff = _speedFactor * timeElapsed;
        //_cubeObj->orbit((float)glm::radians(0.618f * angleDiff), CubeOrbitAxis, glm::vec3(0.f, 3.f, 0.f));
        //_bigTorusObj->rotate((float)glm::radians(angleDiff), BigTorusRotationAxis);
        //_smallTorusObj->orbit((float)glm::radians(-1.f * angleDiff), SmallTorusRotationAxis, glm::vec3(0.f, 0.f, 0.f), true);
        _tetrahedronObj->transform.rotate((float)glm::radians(1.5f * angleDiff), TetrahedronRotationAxis, true);
        _cubeObj->transform.rotate((float)glm::radians(1.5f * angleDiff), TetrahedronRotationAxis, true);
        //_tetrahedronObj->orbit((float)glm::radians(1.5f * angleDiff), TetrahedronOrbitAxis, glm::vec3(0.f), true);
    }
    else
    {
        //torus->lookAt(_camera->getPosition());
    }
}

void LightingSandboxScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    if (_autoRotate)
    {
        if (key == Key::Up && (action == Action::Press || action == Action::Repeat) && (mods & Mod::Control) && _speedFactor < 200)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == Key::Down && (action == Action::Press || action == Action::Repeat) && (mods & Mod::Control) && _speedFactor > 10)
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