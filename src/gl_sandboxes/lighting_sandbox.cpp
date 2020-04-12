#include "lighting_sandbox.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

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

void LightingSandbox::run(GLFWwindow* window)
{
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // Remove cursor from window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), glAspectRatio(), 0.1f, 100.0f);
    CameraPtr _camera = std::make_shared<Camera>(projection, -135.f, -25.f);
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    // Initialize scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->init();

    // Retrieve the custom window pointer, register the scene as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(scene);

    // BIG TORUS
    std::shared_ptr<SceneObject> bigTorusObj = generateSceneMesh(scene, std::make_shared<TorusGenerator>(2.f, 0.5f, 72, 48), Materials::Emerald, lightingShader);
    scene->registerObject(bigTorusObj);

    // SMALL TORUS
    std::shared_ptr<SceneObject> smallTorusObj = generateSceneMesh(scene, std::make_shared<TorusGenerator>(0.75f, 0.25f, 64, 32), Materials::Gold, lightingShader);
    scene->registerObject(smallTorusObj, bigTorusObj->id);
    smallTorusObj->rotate(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    smallTorusObj->translate(glm::vec3(-2.f, 0.f, 0.f));
    
    // AXES
    std::shared_ptr<SceneObject> axesObj = generateSceneMesh(scene, std::make_shared<AxesGenerator>(3.f));
    scene->registerObject(axesObj);
    
    // CUBE
    std::shared_ptr<SceneObject> cubeObj = generateSceneMesh(scene, std::make_shared<CubeGenerator>());
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(5000.f);
    cubeObj->addComponent<LightComponent>(light);
    cubeObj->setPosition(glm::vec3(-3.f, 3.f, 0.f));
    scene->registerObject(cubeObj);
    
    // TETRAHEDRON
    std::shared_ptr<SceneObject> tetrahedronObj = generateSceneMesh(scene, std::make_shared<TetrahedronGenerator>(0.5f));
    scene->registerObject(tetrahedronObj, smallTorusObj->id);
    tetrahedronObj->translate(glm::vec3(1.2f, 0.f, 0.f));
    tetrahedronObj->rotate(glm::radians(90.f), glm::vec3(0.F, 0.f, 1.f));
    
    // CAMERA
    std::shared_ptr<SceneObject> cameraObj = std::make_shared<SceneObject>(scene);
    cameraObj->addComponent<CameraComponent>(_camera);
    cameraObj->setPosition(StartingCameraPosition);
    scene->registerObject(cameraObj);
    std::shared_ptr<FPSCameraScript> fpsScript = std::make_shared<FPSCameraScript>();
    std::shared_ptr<InputProcessingScript> baseFpsScript = std::static_pointer_cast<InputProcessingScript>(fpsScript);
    cameraObj->addComponent<InputProcessingScriptComponent>(baseFpsScript);
    
    // ROTATION SCRIPT
    std::shared_ptr<LightingSandboxScript> rotationScript = std::make_shared<LightingSandboxScript>(cubeObj, bigTorusObj, smallTorusObj, tetrahedronObj);
    std::shared_ptr<InputProcessingScript> ipRotationScript = std::static_pointer_cast<InputProcessingScript>(rotationScript);
    scene->registerInputProcessingScript(ipRotationScript);
    
    // WINDOW SCRIPT
    std::shared_ptr<BasicInputManager> windowScript = std::make_shared<BasicInputManager>();
    std::shared_ptr<InputProcessingScript> ipWindowScript = std::static_pointer_cast<InputProcessingScript>(windowScript);
    scene->registerInputProcessingScript(ipWindowScript);

    SceneRenderer sceneRenderer;
    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and draw scene
        scene->triggerUpdate();
        sceneRenderer.renderScene(scene);

        // Refresh screen and process input
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwSetWindowShouldClose(window, false);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    scene->detachInputProcessingScript(ipRotationScript);

    windowHandler->detachInputProcessor();
}

std::shared_ptr<SceneObject> LightingSandbox::generateSceneMesh(std::shared_ptr<Scene> scene, std::shared_ptr<MeshGenerator> generator, Material mat, Shader shader)
{
    MeshPtr mesh = generator->generatePtr();
    std::shared_ptr<SceneObject> obj = std::make_shared<SceneObject>(SceneWPtr(scene));
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}

LightingSandboxScript::LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr bigTorusObj, SceneObjectPtr smallTorusObj, SceneObjectPtr tetrahedronObj) :
    _cubeObj(cubeObj),
    _bigTorusObj(bigTorusObj),
    _smallTorusObj(smallTorusObj),
    _tetrahedronObj(tetrahedronObj),
    _autoRotate(true),
    _speedFactor(10.f)
{

}

void LightingSandboxScript::update(float timeElapsed)
{
    if (_autoRotate)
    {
        // Update object transforms
        float angleDiff = _speedFactor * timeElapsed;
        _cubeObj->orbit((float)glm::radians(0.618f * angleDiff), CubeOrbitAxis, glm::vec3(0.f, 3.f, 0.f));
        _bigTorusObj->rotate((float)glm::radians(angleDiff), BigTorusRotationAxis);
        _smallTorusObj->orbit((float)glm::radians(-1.f * angleDiff), SmallTorusRotationAxis, glm::vec3(0.f, 0.f, 0.f), true);
        //_tetrahedronObj->rotate((float)glm::radians(1.5f * angleDiff), TetrahedronRotationAxis, true);
        _tetrahedronObj->orbit((float)glm::radians(1.5f * angleDiff), TetrahedronOrbitAxis, glm::vec3(0.f), true);
    }
    else
    {
        //torus->lookAt(_camera->getPosition());
    }
}

void LightingSandboxScript::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (_autoRotate)
    {
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor < 200)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && (mods & GLFW_MOD_CONTROL) && _speedFactor > 10)
        {
            _speedFactor /= 1.1f;
        }
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        _autoRotate = !_autoRotate;
    }
}

LightingSandboxScript* LightingSandboxScript::clone()
{
    return nullptr;
}