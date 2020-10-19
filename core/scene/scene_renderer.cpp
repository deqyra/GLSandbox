#include "scene_renderer.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "scene.hpp"
#include "scene_object.hpp"
#include "component.hpp"
#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

#include "../lights/light.hpp"
#include "../ubo/matrix_ubo.hpp"
#include "../ubo/light_ubo.hpp"

#include "../mesh.hpp"
#include "../material.hpp"
#include "../shader.hpp"

SceneRenderer::SceneRenderer() :
    _matrixUbo(),
    _lightUbo()
{

}

void SceneRenderer::renderScene(ScenePtr scene)
{
    scene->updateAllTransforms();

    // Get pointers to meshes, lights, and the scene camera
    std::vector<SceneObjectPtr> meshComponents = scene->getObjectsWithComponent<MeshComponent>();
    std::vector<SceneObjectPtr> lightComponents = scene->getObjectsWithComponent<LightComponent>();
    std::vector<SceneObjectPtr> cameraComponents = scene->getObjectsWithComponent<CameraComponent>();

    if (cameraComponents.size() == 0)
    {
        std::cout << "SceneRenderer: No camera in provided scene, cannot render scene." << std::endl;
        return;
    }
    if (cameraComponents.size() > 1)
    {
        std::cout << "SceneRenderer: More than one camera in provided scene, the first one will be used for rendering." << std::endl;
    }

    // Get the actual camera
    SceneObjectPtr cameraObj = cameraComponents[0];
    std::shared_ptr<CameraComponent> cameraComp = cameraObj->getComponent<CameraComponent>();

    // Set up matrices in their UBO
    glm::mat4 view = cameraComp->getViewMatrix();
    glm::mat4 projection = cameraComp->getProjectionMatrix();
    _matrixUbo.setView(view);
    _matrixUbo.setProjection(projection);

    // Preprocess lights
    std::vector<LightPtr> lights;
    std::vector<Transform> worldTransforms;
    for (auto it = lightComponents.begin(); it != lightComponents.end(); it++)
    {
        // Get the light component
        SceneObjectPtr lightObj = *it;
        std::shared_ptr<LightComponent> lightComp = lightObj->getComponent<LightComponent>();
        // Get the actual light and its world model matrix (needed to compute its world position)
        lights.push_back(lightComp->getLight());
        worldTransforms.push_back(scene->getWorldTransform(lightObj->id));
    }
    sendLightData(lights, worldTransforms, view);

    for (auto it = meshComponents.begin(); it != meshComponents.end(); it++)
    {
        // Get the mesh component
        SceneObjectPtr meshObj = *it;
        std::shared_ptr<MeshComponent> meshComp = meshObj->getComponent<MeshComponent>();
        // Draw the mesh
        glm::mat4 modelMatrix = scene->getWorldTransform(meshObj->id).getModelMatrix();
        drawMesh(meshComp->mesh, modelMatrix, cameraComp->getViewMatrix(), meshComp->material, meshComp->shader);
    }
}

void SceneRenderer::sendLightData(std::vector<LightPtr>& lights, std::vector<Transform>& worldTransforms, glm::mat4 view)
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

void SceneRenderer::drawMesh(MeshPtr mesh, glm::mat4 model, glm::mat4 view, Material material, Shader shader)
{
    // Compute normal matrix (restore normals after non-uniform transforms)
    glm::mat4 normal = glm::transpose(glm::inverse(view * model));
    // Set up matrices in UBO
    _matrixUbo.setModel(model);
    _matrixUbo.setNormal(normal);

    // Set up shader and material
    shader.use();
    material.bindTextures();
    shader.setMaterial("material", material);

    mesh->draw();
}
