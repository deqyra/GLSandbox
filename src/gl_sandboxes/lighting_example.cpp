/**
    GLTest, perspective_triangle_example.cpp
    Purpose: Implementation of class RotatingTexturedCubeExample. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 05/02/2020
 */
#include "lighting_example.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/mesh.hpp"
#include "../core/materials.hpp"
#include "../core/shader.hpp"
#include "../core/lights/point_light.hpp"

#include "../core/mesh_generators/torus_generator.hpp"
#include "../core/mesh_generators/axes_generator.hpp"
#include "../core/mesh_generators/cube_generator.hpp"

#include "../tools/gl_utils.hpp"
#include "../tools/gl_window.hpp"

#include "../core/scene.hpp"
#include "../core/scene_renderer.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/scene_object_component_type.hpp"

#define CAMERA_POS glm::vec3(5.f, 3.f, 5.f)
#define CUBE_ROTATION_AXIS glm::vec3(0.f, 1.f, 0.f)
#define TORUS_ROTATION_AXIS glm::vec3(1.f, 0.f, 0.f)

LightingSandbox::LightingSandbox() :
    _angle(0.f),
    _autoRotate(true),
    _speedFactor(10.f)
{
    _lastTime = (float)glfwGetTime();
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
    CameraPtr camera = std::make_shared<Camera>(projection, -135.f, -25.f);
    _lastTime = (float)glfwGetTime();
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    // Initialize scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->init();

    // Retrieve the custom window pointer, register the scene as an input processor
    GLWindow* windowHandler = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    windowHandler->registerInputProcessor(scene);

    // TORUS
    std::shared_ptr<SceneObject> torusObj = generateSceneMesh(scene, std::make_shared<TorusGenerator>(2.f, 0.5f, 72, 48), Materials::Emerald, lightingShader);
    scene->registerObject(torusObj);
    // AXES
    std::shared_ptr<SceneObject> axesObj = generateSceneMesh(scene, std::make_shared<AxesGenerator>(3.f));
    scene->registerObject(axesObj);
    // CUBE
    std::shared_ptr<SceneObject> cubeObj = generateSceneMesh(scene, std::make_shared<CubeGenerator>());
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>();
    cubeObj->addComponent<LightComponent>(light);
    cubeObj->setPosition(glm::vec3(-3.f, 3.f, 0.f));
    scene->registerObject(cubeObj);
    // CAMERA
    std::shared_ptr<SceneObject> cameraObj = std::make_shared<SceneObject>(scene);
    cameraObj->addComponent<CameraComponent>(camera);
    cameraObj->setPosition(CAMERA_POS);
    scene->registerObject(cameraObj);

    SceneRenderer sceneRenderer;
    while (!glfwWindowShouldClose(window))
    {
        float frameTime = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_autoRotate)
        {
            // Update object transforms
            float angleDiff = _speedFactor * (frameTime - _lastTime);
            cubeObj->orbit((float)glm::radians(0.618 * angleDiff), CUBE_ROTATION_AXIS, glm::vec3(0.f, 3.f, 0.f));
            torusObj->rotate((float)glm::radians(angleDiff), TORUS_ROTATION_AXIS);
        }
        else
        {
            //torus->lookAt(_camera->getPosition());
        }

        // Draw meshes
        sceneRenderer.renderScene(scene);

        // Refresh screen and process input
        glfwSwapBuffers(window);
        glfwPollEvents();

        _lastTime = frameTime;
    }
    glfwSetWindowShouldClose(window, false);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  

    windowHandler->removeInputProcessor();
}

void LightingSandbox::handleKeyboardObjectRotation(GLFWwindow* window, int key, int scancode, int action, int mods)
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

std::shared_ptr<SceneObject> LightingSandbox::generateSceneMesh(std::shared_ptr<Scene> scene, std::shared_ptr<MeshGenerator> generator, Material mat, Shader shader)
{
    MeshPtr mesh = generator->generatePtr();
    std::shared_ptr<SceneObject> obj = std::make_shared<SceneObject>(SceneWPtr(scene));
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}
