#include "shadow_sandbox.hpp"

#include <string>
#include <glad/gl.h>

#include <renderboi/core/camera.hpp>
#include <renderboi/core/factory.hpp>
#include <renderboi/core/frame_of_reference.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/texture_2d.hpp>
#include <renderboi/core/pixel_space.hpp>
#include <renderboi/core/shader/shader.hpp>

#include <renderboi/core/mesh_generators/mesh_type.hpp>
#include <renderboi/core/mesh_generators/plane_generator.hpp>
#include <renderboi/core/mesh_generators/torus_generator.hpp>

#include <renderboi/core/scene/scene.hpp>
#include <renderboi/core/scene/scene_object.hpp>
#include <renderboi/core/scene/scene_renderer.hpp>

#include <renderboi/core/scripts/basic_input_manager.hpp>
#include <renderboi/core/scripts/mouse_camera_script.hpp>
#include <renderboi/core/scripts/keyboard_movement_script.hpp>
#include <renderboi/core/scripts/camera_aspect_ratio_script.hpp>

using Ref = FrameOfReference;

void ShadowSandbox::run(GLWindowPtr window)
{
    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Shadows");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);
    
    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderInfo::ShaderFeature::VertexMVP);
    //lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentMeshMaterial);
    //lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentBlinnPhong);
    lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentFullColor);
    //lightConfig.addFeature(ShaderInfo::ShaderFeature::FragmentGammaCorrection);
    ShaderProgram lightingShader = ShaderBuilder::buildShaderProgramFromConfig(lightConfig);
    lightingShader.setFloat("gamma", 1.7f);

    ShaderConfig depthConfig;
    depthConfig.addFeature(ShaderInfo::ShaderFeature::VertexMVP);
    depthConfig.addFeature(ShaderInfo::ShaderFeature::FragmentDepthView);
    ShaderProgram depthShader = ShaderBuilder::buildShaderProgramFromConfig(depthConfig);

    ScenePtr scene = Factory::makeScene();

    // Register the scene as an input processor to the window
    window->registerInputProcessor(scene);

    PlaneGenerator::Parameters planeParameters = {
        PlaneTileSize,      // tileSizeX
        PlaneTileSize,      // tileSizeY
        PlaneTileCount,     // tileAmountX
        PlaneTileCount,     // tileAmountY
        5.f,                // xTexSize
        5.f,                // yTexSize
        0.f,                // xTexCoordOffset
        0.f,                // yTexCoordOffset
        false,              // invertXTexCoords
        false,              // invertYTexCoords
        0.f,                // texRotation
        {1.f, 1.f, 1.f}     // color
    };

    // FLOOR
    Material floorMaterial = Material(
        glm::vec3(0.f),
        glm::vec3(1.f),
        glm::vec3(0.3203125f, 0.254296875f, 0.180859375f),
        128.f
    );
    Texture2D floorTex = Texture2D("assets/textures/wood.png", PixelSpace::sRGB);
    floorMaterial.pushDiffuseMap(floorTex);
    SceneObjectPtr floorObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("Floor", planeParameters, floorMaterial, lightingShader);

    // WALLS
    Material wallMaterial = Material(
        glm::vec3(0.f), 
        glm::vec3(1.f), 
        glm::vec3(0.25f, 0.15f, 0.15f), 
        2.f
    );
    Texture2D wallTex = Texture2D("assets/textures/wall.jpg", PixelSpace::sRGB);
    wallMaterial.pushDiffuseMap(wallTex);

    // XY wall
    SceneObjectPtr xyWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("XY wall", planeParameters, wallMaterial, lightingShader);

    // YZ wall
    SceneObjectPtr yzWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("YZ wall", planeParameters, wallMaterial, lightingShader);

    TorusGenerator::Parameters torusParameters = {
        4.f,    // toroidalRadius
        0.5f,   // poloidalRadius
        96,     // toroidalVertexRes
        48      // poloidalVertexRes
    };

    // TORUS
    SceneObjectPtr torusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Torus", torusParameters, Materials::Default, lightingShader);

    // LIGHT
    SceneObjectPtr lightObj = Factory::makeSceneObjectWithMesh<MeshType::Cube>("Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Materials::Default, lightingShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(LightBaseRange);
    lightObj->addComponent<LightComponent>(light);

    // CAMERA
    SceneObjectPtr cameraObj = Factory::makeSceneObject("Camera");
    CameraPtr camera = std::make_shared<Camera>(CameraParams);
    cameraObj->addComponent<CameraComponent>(camera);

    depthShader.setFloat("near", camera->getNearDistance());
    depthShader.setFloat("far", camera->getFarDistance());

    // Register all objects
    scene->registerObject(floorObj);
    scene->registerObject(xyWallObj);
    scene->registerObject(yzWallObj);
    scene->registerObject(torusObj);
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
    torusObj->transform.translateBy<Ref::World>({WallSize / 2.f, WallSize / 2.f, WallSize / 2.f});
    torusObj->transform.rotateBy<Ref::World>(glm::radians(45.f), X);
    lightObj->transform.setPosition<Ref::World>(LightPosition);
    cameraObj->transform.setPosition<Ref::World>(StartingCameraPosition);
    cameraObj->transform.rotateBy<Ref::Parent>(glm::radians(180.f), Y);

    // MOVEMENT SCRIPT
    std::shared_ptr<ShadowSandboxScript> movementScript = std::make_shared<ShadowSandboxScript>(lightObj, torusObj);
    std::shared_ptr<InputProcessingScript> ipMovementScript = std::static_pointer_cast<InputProcessingScript>(movementScript);
    scene->registerInputProcessingScript(ipMovementScript);
    
    // WINDOW SCRIPT
    std::shared_ptr<BasicInputManager> windowScript = std::make_shared<BasicInputManager>();
    std::shared_ptr<InputProcessingScript> ipWindowScript = std::static_pointer_cast<InputProcessingScript>(windowScript);
    scene->registerInputProcessingScript(ipWindowScript);

    SceneRenderer sceneRenderer;

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    while (!window->shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

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

ShadowSandboxScript::ShadowSandboxScript(SceneObjectPtr lightObj, SceneObjectPtr torusObj) :
    _lightObj(lightObj),
    _torusObj(torusObj),
    _lightStartingPos(lightObj->transform.getPosition()),
    _pause(false),
    _speedFactor(2.f),
    _sine(LightMovementFrequency)
{

}

void ShadowSandboxScript::update(float timeElapsed)
{
    static bool firstUpdate = true;
    if (firstUpdate)
    {
        _sine.start();
        firstUpdate = false;
    }

    if (!_pause)
    {
        glm::vec3 newLightPos = _lightStartingPos + LightMovementAxis * _sine.get() * (LightMovementAmplitude / 2.f);
        _lightObj->transform.setPosition<Ref::World>(newLightPos);

        float delta = _speedFactor * timeElapsed;
        _torusObj->transform.rotateBy<Ref::World>(glm::radians(45.f * delta), TorusRotationAxis);
    }
}

void ShadowSandboxScript::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    if (!_pause)
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
        _pause = !_pause;

        if (_pause) _sine.pause();
        else _sine.start();
    }
}

ShadowSandboxScript* ShadowSandboxScript::clone()
{
    return nullptr;
}