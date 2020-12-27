#include "shadow_sandbox.hpp"

#include <string>
#include <glad/gl.h>

#include <renderboi/core/camera.hpp>
#include <renderboi/core/frame_of_reference.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/texture_2d.hpp>
#include <renderboi/core/pixel_space.hpp>
#include <renderboi/core/shader/shader.hpp>
#include <renderboi/core/lights/point_light.hpp>

#include <renderboi/toolbox/factory.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/input_logger.hpp>
#include <renderboi/toolbox/controls/controlled_entity_manager.hpp>
#include <renderboi/toolbox/mesh_generators/mesh_type.hpp>
#include <renderboi/toolbox/mesh_generators/plane_generator.hpp>
#include <renderboi/toolbox/mesh_generators/torus_generator.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/scene_object.hpp>
#include <renderboi/toolbox/scene/scene_renderer.hpp>
#include <renderboi/toolbox/scene/components/camera_component.hpp>
#include <renderboi/toolbox/scene/components/light_component.hpp>
#include <renderboi/toolbox/scene/components/mesh_component.hpp>
#include <renderboi/toolbox/runnables/basic_window_manager.hpp>
#include <renderboi/toolbox/runnables/camera_aspect_ratio_manager.hpp>
#include <renderboi/toolbox/runnables/mouse_camera_manager.hpp>
#include <renderboi/toolbox/runnables/keyboard_movement_script.hpp>

#include <cpptools/enum_map.hpp>

using Ref = FrameOfReference;

void ShadowSandbox::run(GLWindowPtr window)
{
    // Update window title
    std::string title = window->getTitle();
    window->setTitle(title + " - Shadows");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);



    ////////////////////////////
    ///                      ///
    ///   Generate shaders   ///
    ///                      ///
    ////////////////////////////

    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderFeature::VertexMVP);
    lightConfig.addFeature(ShaderFeature::FragmentFullLight);
    ShaderProgram fullLightShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);

    lightConfig.removeFeature(ShaderFeature::FragmentFullLight);
    lightConfig.addFeature(ShaderFeature::FragmentMeshMaterial);
    lightConfig.addFeature(ShaderFeature::FragmentBlinnPhong);
    //lightConfig.addFeature(ShaderFeature::FragmentFullLight);
    lightConfig.addFeature(ShaderFeature::FragmentGammaCorrection);
    ShaderProgram blinnPhongShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);

    //ShaderConfig depthConfig;
    //depthConfig.addFeature(ShaderFeature::VertexMVP);
    //depthConfig.addFeature(ShaderFeature::FragmentViewDepthBuffer);
    //ShaderProgram depthShader = ShaderBuilder::BuildShaderProgramFromConfig(depthConfig);
    


    /////////////////////////////////////////////
    ///                                       ///
    ///   Create scene, instantiate objects   ///
    ///                                       ///
    /////////////////////////////////////////////

    ScenePtr scene = Factory::makeScene();

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
    SceneObjectPtr floorObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("Floor", planeParameters, floorMaterial, blinnPhongShader);

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
    SceneObjectPtr xyWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("XY wall", planeParameters, wallMaterial, blinnPhongShader);

    // YZ wall
    SceneObjectPtr yzWallObj = Factory::makeSceneObjectWithMesh<MeshType::Plane>("YZ wall", planeParameters, wallMaterial, blinnPhongShader);

    TorusGenerator::Parameters torusParameters = {
        4.f,    // toroidalRadius
        0.5f,   // poloidalRadius
        96,     // toroidalVertexRes
        48      // poloidalVertexRes
    };

    // TORUS
    SceneObjectPtr torusObj = Factory::makeSceneObjectWithMesh<MeshType::Torus>("Torus", torusParameters, Materials::Default, blinnPhongShader);

    // LIGHT
    SceneObjectPtr lightObj = Factory::makeSceneObjectWithMesh<MeshType::Cube>("Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Materials::Default, fullLightShader);
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(LightBaseRange);
    lightObj->addComponent<LightComponent>(light);

    // CAMERA
    SceneObjectPtr cameraObj = Factory::makeSceneObject("Camera");
    CameraPtr camera = std::make_shared<Camera>(CameraParams);
    cameraObj->addComponent<CameraComponent>(camera);

    //depthShader.setFloat("near", camera->getNearDistance());
    //depthShader.setFloat("far", camera->getFarDistance());

    // Register all objects
    scene->registerObject(floorObj);
    scene->registerObject(xyWallObj);
    scene->registerObject(yzWallObj);
    scene->registerObject(torusObj);
    scene->registerObject(lightObj);
    scene->registerObject(cameraObj);



    ///////////////////////////////
    ///                         ///
    ///   Move objects around   ///
    ///                         ///
    ///////////////////////////////

    const glm::vec3 X = Transform::X;
    const glm::vec3 Y = Transform::Y;
    const glm::vec3 Z = Transform::Z;

    floorObj->transform.rotateBy<Ref::World>(glm::radians(-90.f), X);
    floorObj->transform.translateBy<Ref::World>({0.f, 0.f, WallSize});
    yzWallObj->transform.rotateBy<Ref::World>(glm::radians(90.f), Y);
    yzWallObj->transform.translateBy<Ref::World>({0.f, 0.f, WallSize});
    torusObj->transform.translateBy<Ref::World>({WallSize / 2.f, WallSize / 2.f, WallSize / 2.f});
    torusObj->transform.rotateBy<Ref::World>(glm::radians(45.f), X);
    lightObj->transform.setPosition<Ref::World>(LightPosition);
    cameraObj->transform.setPosition<Ref::World>(StartingCameraPosition);
    cameraObj->transform.rotateBy<Ref::Parent>(glm::radians(180.f), Y);



    /////////////////////////////////////
    ///                               ///
    ///   Add and configure scripts   ///
    ///                               ///
    /////////////////////////////////////

    // Link camera to MouseCameraManager
    std::shared_ptr<MouseCameraManager> cameraManager = std::make_shared<MouseCameraManager>(camera);

    // Link camera to CameraAspectRatioManager
    std::shared_ptr<CameraAspectRatioManager> cameraAspectRatioManager = std::make_shared<CameraAspectRatioManager>(camera);

    // Attach object movement script to scene
    std::shared_ptr<ShadowSandboxScript> movementScript = std::make_shared<ShadowSandboxScript>(lightObj, torusObj);
    scene->registerScript(std::static_pointer_cast<Script>(movementScript));
    
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
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(movementScript));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(keyboardScriptManager.getEventTranslator()));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(windowManager.getEntity()));
    splitter->registerInputProcessor(std::static_pointer_cast<InputProcessor>(windowManager.getEventTranslator()));
    
    // Register the splitter to the window
    window->registerInputProcessor(std::static_pointer_cast<InputProcessor>(splitter));



    //////////////////////////
    ///                    ///
    ///   Rendering loop   ///
    ///                    ///
    //////////////////////////

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



    ////////////////////
    ///              ///
    ///   Clean up   ///
    ///              ///
    ////////////////////

    splitter->detachAllInputProcessors();
    Factory::destroyScene(scene);

    // Reset everything back to how it was
    window->setInputMode(InputMode::Target::Cursor, InputMode::Value::NormalCursor);
    window->detachInputProcessor();
    window->setTitle(title);
}

ShadowSandboxScript::ShadowSandboxScript(SceneObjectPtr lightObj, SceneObjectPtr torusObj) :
    _lightObj(lightObj),
    _torusObj(torusObj),
    _lightStartingPos(lightObj->getWorldTransform().getPosition()),
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
        _pause = !_pause;

        if (_pause) _sine.pause();
        else _sine.start();
    }
}

ShadowSandboxScript* ShadowSandboxScript::clone()
{
    return nullptr;
}