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

void SceneRenderer::renderScene(SceneWPtr wScene)
{
    ScenePtr scene = wScene.lock();

    // Get pointers to meshes, lights, and the scene camera
    std::vector<SceneObjectWPtr> meshComponents = scene->getObjectsWithComponent<MeshComponent>();
    std::vector<SceneObjectWPtr> lightComponents = scene->getObjectsWithComponent<LightComponent>();
    std::vector<SceneObjectWPtr> cameraComponents = scene->getObjectsWithComponent<CameraComponent>();

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
    SceneObjectPtr cameraObj = cameraComponents[0].lock();
    std::shared_ptr<CameraComponent> cameraComp = cameraObj->getComponent<CameraComponent>().lock();

    // Set up matrices in their UBO
    glm::mat4 view = cameraComp->getViewMatrix();
    glm::mat4 projection = cameraComp->getProjectionMatrix();
    _matrixUbo.setView(view);
    _matrixUbo.setProjection(projection);

    // Preprocess lights
    std::vector<LightPtr> lights;
    std::vector<glm::mat4> modelMats;
    for (auto it = lightComponents.begin(); it != lightComponents.end(); it++)
    {
        // Get the light component
        SceneObjectPtr lightObj = it->lock();
        std::shared_ptr<LightComponent> lightComp = lightObj->getComponent<LightComponent>().lock();
        // Get the actual light and its world model matrix (needed to compute its world position)
        lights.push_back(lightComp->light);
        modelMats.push_back(scene->getWorldModelMatrix(lightObj->id));
    }
    sendLightData(lights, modelMats, view);

    for (auto it = meshComponents.begin(); it != meshComponents.end(); it++)
    {
        // Get the mesh component
        SceneObjectPtr meshObj = it->lock();
        std::shared_ptr<MeshComponent> meshComp = meshObj->getComponent<MeshComponent>().lock();
        // Draw the mesh
        glm::mat4 modelMatrix = scene->getWorldModelMatrix(meshObj->id);
        drawMesh(meshComp->mesh, modelMatrix, cameraComp->getViewMatrix(), meshComp->material, meshComp->shader);
    }
}

void SceneRenderer::sendLightData(std::vector<LightPtr>& lights, std::vector<glm::mat4>& modelMats, glm::mat4 view)
{
    // Light counts need to be kept track of
    unsigned int pLightIndex = 0;
    unsigned int sLightIndex = 0;
    unsigned int dLightIndex = 0;

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        LightPtr light = lights[i];
        switch (light->lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < POINT_LIGHT_MAX_COUNT)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<PointLight> pLight = std::static_pointer_cast<PointLight>(light);
                    // Send it to the UBO
                    glm::vec4 position = glm::vec4(glm::vec3(0.f), 1.f);
                    position = view * modelMats[i] * position;
                    _lightUbo.setPoint(pLightIndex++, *pLight, glm::vec3(position));
                }
                break;
            case LightType::SpotLight:
                if (sLightIndex < SPOT_LIGHT_MAX_COUNT)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<SpotLight> sLight = std::static_pointer_cast<SpotLight>(light);
                    // Send it to the UBO
                    glm::vec4 position = glm::vec4(glm::vec3(0.f), 1.f);
                    position = view * modelMats[i] * position;
                    _lightUbo.setSpot(sLightIndex++, *sLight, glm::vec3(position));
                }
                break;
            case LightType::DirectionalLight:
                if (dLightIndex < DIRECTIONAL_LIGHT_MAX_COUNT)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<DirectionalLight> dLight = std::static_pointer_cast<DirectionalLight>(light);
                    // Send it to the UBO
                    _lightUbo.setDirectional(dLightIndex++, *dLight);
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
