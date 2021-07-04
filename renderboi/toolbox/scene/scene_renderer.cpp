#include "scene_renderer.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <renderboi/core/lights/light.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/mesh.hpp>
#include <renderboi/core/shader/shader_program.hpp>
#include <renderboi/core/transform.hpp>
#include <renderboi/core/ubo/matrix_ubo.hpp>
#include <renderboi/core/ubo/light_ubo.hpp>

#include "scene.hpp"
#include "scene_object.hpp"
#include "component.hpp"
#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

namespace Renderboi
{

SceneRenderer::SceneRenderer(const unsigned int framerateLimit) :
    _matrixUbo(),
    _lightUbo(),
    _frameIntervalUs((int64_t)(1000000.f/framerateLimit)),
    _lastTimestamp(std::chrono::system_clock::now())
{

}

void SceneRenderer::renderScene(const ScenePtr scene) const
{
    scene->updateAllTransforms();

    // Get pointers to meshes, lights, and the scene camera
    const std::vector<SceneObjectPtr> meshObjects = scene->getObjectsWithComponent<MeshComponent>();
    const std::vector<SceneObjectPtr> lightObjects = scene->getObjectsWithComponent<LightComponent>();
    const std::vector<SceneObjectPtr> cameraObjects = scene->getObjectsWithComponent<CameraComponent>();

    if (cameraObjects.size() == 0)
    {
        std::cout << "SceneRenderer: No camera in provided scene, cannot render scene." << std::endl;
        return;
    }
    if (cameraObjects.size() > 1)
    {
        std::cout << "SceneRenderer: More than one camera in provided scene, "
            "the first one will be used for rendering." << std::endl;
    }

    // Get the actual camera
    const SceneObjectPtr cameraObj = cameraObjects[0];
    const std::shared_ptr<CameraComponent> cameraComp = cameraObj->getComponent<CameraComponent>();

    // Set up matrices in their UBO
    const glm::mat4 view = cameraComp->getViewMatrix();
    const glm::mat4 projection = cameraComp->getProjectionMatrix();
    _matrixUbo.setView(view);
    _matrixUbo.setProjection(projection);

    // Preprocess lights
    std::vector<LightPtr> lights;
    std::vector<Transform> worldTransforms;
    for (const auto& lightObj : lightObjects)
    {
        std::shared_ptr<LightComponent> lightComp = lightObj->getComponent<LightComponent>();
        // Get the actual light and its world model matrix (needed to compute its world position)
        lights.push_back(lightComp->getLight());
        worldTransforms.push_back(scene->getWorldTransform(lightObj->id));
    }
    _sendLightData(lights, worldTransforms, view);

    // Compute time elapsed between frames and limit framerate if necessary
    const std::chrono::time_point<std::chrono::system_clock> newTimestamp = std::chrono::system_clock::now();
    const std::chrono::duration<double> duration = newTimestamp - _lastTimestamp;
    _lastTimestamp = newTimestamp;
    const int64_t gap = _frameIntervalUs - std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::this_thread::sleep_for(std::chrono::microseconds(gap));

    for (const auto& meshObj : meshObjects)
    {
        // Draw the mesh
        drawMesh(meshObj, cameraComp->getViewMatrix());
    }
}

void SceneRenderer::setFramerateLimit(const unsigned int framerateLimit)
{
    _frameIntervalUs = (int64_t)(1000000.f / framerateLimit);
}

void SceneRenderer::_sendLightData(
    const std::vector<LightPtr>& lights,
    const std::vector<Transform>& worldTransforms,
    const glm::mat4& view
) const
{
    // Light counts need to be kept track of
    unsigned int pLightIndex = 0;
    unsigned int sLightIndex = 0;
    unsigned int dLightIndex = 0;

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        LightPtr light = lights[i];
        glm::vec3 position = glm::vec3(view * glm::vec4(worldTransforms[i].getPosition(), 1.f));

        switch (light->lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < LightUBO::PointLightMaxCount)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<PointLight> pLight = std::static_pointer_cast<PointLight>(light);
                    // Send it to the UBO
                    _lightUbo.setPoint(pLightIndex++, *pLight, position);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: PointLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
            case LightType::SpotLight:
                if (sLightIndex < LightUBO::SpotLightMaxCount)
                {
                    // Retrieve the light as a SpotLight
                    std::shared_ptr<SpotLight> sLight = std::static_pointer_cast<SpotLight>(light);
                    // Send it to the UBO
                    _lightUbo.setSpot(sLightIndex++, *sLight, position);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: SpotLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
            case LightType::DirectionalLight:
                if (dLightIndex < LightUBO::DirectionalLightMaxCount)
                {
                    // Retrieve the light as a DirectionalLight
                    std::shared_ptr<DirectionalLight> dLight = std::static_pointer_cast<DirectionalLight>(light);
                    // Send it to the UBO
                    _lightUbo.setDirectional(dLightIndex++, *dLight);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: DirectionalLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
        }
    }

    // Set light counts
    _lightUbo.setPointCount(pLightIndex);
    _lightUbo.setSpotCount(sLightIndex);
    _lightUbo.setDirectionalCount(dLightIndex);
}

void SceneRenderer::drawMesh(const SceneObjectPtr meshObject, const glm::mat4& viewMatrix) const
{
    const Transform objectTransform = meshObject->getWorldTransform();
    const glm::mat4 modelMatrix = objectTransform.getModelMatrix();

    // Detect non uniform scaling: compute the dot product of the world scale
    // of the object and a uniform scale along all three axes. If the dot 
    // product is not 1, then the object has non-uniform scaling.
    const glm::vec3 scaling = glm::normalize(objectTransform.getScale());
    const float dot = glm::dot(scaling, glm::normalize(glm::vec3(1.f, 1.f, 1.f)));

    // Compute normal matrix
    glm::mat4 normalMatrix = viewMatrix * modelMatrix;
    if (1.f - glm::abs(dot) > 1.e-6)
    {
        // Restore normals if a non-uniform scaling was detected
        normalMatrix = glm::transpose(glm::inverse(normalMatrix));
    }

    // Set up matrices in UBO
    _matrixUbo.setModel(modelMatrix);
    _matrixUbo.setNormal(normalMatrix);

    const std::shared_ptr<MeshComponent> meshComponent = meshObject->getComponent<MeshComponent>();
    
    // Set up shader and material
    const Material material = meshComponent->getMaterial();
    ShaderProgram shader = meshComponent->getShader();
    shader.use();
    material.bindTextures();

    if (shader.supports(ShaderFeature::FragmentMeshMaterial))
    {
        shader.setMaterial("material", material);
    }

    meshComponent->getMesh()->draw();
}

}//namespace Renderboi
