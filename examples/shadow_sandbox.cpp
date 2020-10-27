#include "shadow_sandbox.hpp"

#include <string>
#include <glad/gl.h>

#include "../core/camera.hpp"
#include "../core/factory.hpp"
#include "../core/frame_of_reference.hpp"
#include "../core/shader.hpp"
#include "../core/material.hpp"
#include "../core/texture_2d.hpp"

#include "../core/mesh_generators/mesh_type.hpp"
#include "../core/mesh_generators/plane_generator.hpp"

#include "../core/scene/scene.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/scene_renderer.hpp"

#include "../core/scripts/basic_input_manager.hpp"
#include "../core/scripts/mouse_camera_script.hpp"
#include "../core/scripts/keyboard_movement_script.hpp"
#include "../core/scripts/camera_aspect_ratio_script.hpp"

using Ref = FrameOfReference;

void ShadowSandbox::run(GLWindowPtr window)
{
    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Shadows");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);
    
    Shader lightingShader = Shader("assets/shaders/mvp.vert", "assets/shaders/phong.frag");

    ScenePtr scene = Factory::makeScene();

    // Register the scene as an input processor to the window
    window->registerInputProcessor(scene);

    PlaneGenerator::Parameters planeParameters = {
        PlaneTileSize,      // tileSizeX
        PlaneTileSize,      // tileSizeY
        PlaneTileCount,     // tileAmountX
        PlaneTileCount,     // tileAmountY
        0.f,                // xTexSize
        0.f,                // yTexSize
        0.f,                // xTexCoordOffset
        0.f,                // yTexCoordOffset
        false,              // invertXTexCoords
        false,              // invertYTexCoords
        0.f,                // texRotation
        {1.f, 1.f, 1.f}     // color
    };

    // FLOOR
    Material floorMaterial = Material();
    Texture2D floorTex = Texture2D("assets/textures/wood.png");
    floorMaterial.pushDiffuseMap(floorTex);
    SceneObjectPtr floorObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("Floor", planeParameters, floorMaterial, lightingShader);

    // WALLS
    Material wallMaterial = Material();
    wallMaterial.specular = glm::vec3(0.1f);
    wallMaterial.shininess = 2.f;
    Texture2D wallTex = Texture2D("assets/textures/wall.jpg");
    wallMaterial.pushDiffuseMap(wallTex);

    // XY wall
    SceneObjectPtr xyWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("XY wall", planeParameters, wallMaterial, lightingShader);

    // YZ wall
    SceneObjectPtr yzWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("YZ wall", planeParameters, wallMaterial, lightingShader);

    // LIGHT
    SceneObjectPtr lightObj = Factory::makeSceneObjectWithMesh<MeshType::Cube>("Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Material(), lightingShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(LightBaseRange);
    lightObj->addComponent<LightComponent>(light);

    // CAMERA
    SceneObjectPtr cameraObj = Factory::makeSceneObject("Camera");
    CameraPtr camera = std::make_shared<Camera>(CameraParams);
    cameraObj->addComponent<CameraComponent>(camera);

    // Register all objects
    scene->registerObject(floorObj);
    scene->registerObject(xyWallObj);
    scene->registerObject(yzWallObj);
    scene->registerObject(lightObj);
    scene->registerObject(cameraObj);

    // Add script component to camera: MouseCameraScript
    Factory::createInputProcessingScriptAndAttachToObject<MouseCameraScript>(cameraObj);

    // Add script component to camera: KeyboardMovementScript
    std::shared_ptr<BasisProvider> cameraAsBasisProvider = std::static_pointer_cast<BasisProvider>(camera);
    Factory::createInputProcessingScriptAndAttachToObject<KeyboardMovementScript>(cameraObj, cameraAsBasisProvider);

    // Add script component to camera: CameraAspectRatioScript
    Factory::createInputProcessingScriptAndAttachToObject<CameraAspectRatioScript>(cameraObj);

    const glm::vec3 X = Transform::X;
    const glm::vec3 Y = Transform::Y;
    const glm::vec3 Z = Transform::Z;

    // Move objects around
    floorObj->transform.rotateBy<Ref::World>(glm::radians(-90.f), X);
    floorObj->transform.translateBy<Ref::World>({0.f, 0.f, WallSize});
    yzWallObj->transform.rotateBy<Ref::World>(glm::radians(90.f), Y);
    yzWallObj->transform.translateBy<Ref::World>({0.f, 0.f, WallSize});
    lightObj->transform.setPosition<Ref::World>(LightPosition);
    cameraObj->transform.setPosition<Ref::World>(StartingCameraPosition);
    cameraObj->transform.rotateBy<Ref::Parent>(glm::radians(180.f), Y);

    // WINDOW SCRIPT
    std::shared_ptr<BasicInputManager> windowScript = std::make_shared<BasicInputManager>();
    std::shared_ptr<InputProcessingScript> ipWindowScript = std::static_pointer_cast<InputProcessingScript>(windowScript);
    scene->registerInputProcessingScript(ipWindowScript);

    SceneRenderer sceneRenderer;

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!window->shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and draw scene
        scene->triggerUpdate();

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