#include "scene_renderer.hpp"
#include "scene/components/mesh_component.hpp"
#include "scene/components/light_component.hpp"
#include "scene/components/camera_component.hpp"

#include <iostream>

SceneRenderer::SceneRenderer() :
    _matrixUbo(),
    _lightUbo()
{

}

void SceneRenderer::renderScene(Scene& scene)
{
    std::vector<WeakObjPtr> meshComponents = scene.getObjectsWithComponent<MeshComponent>();
    std::vector<WeakObjPtr> lightComponents = scene.getObjectsWithComponent<LightComponent>();
    std::vector<WeakObjPtr> cameraComponents = scene.getObjectsWithComponent<CameraComponent>();

    if (cameraComponents.size() == 0)
    {
        std::cout << "SceneRenderer: No camera in provided scene, cannot render scene." << std::endl;
        return;
    }
    if (cameraComponents.size() > 1)
    {
        std::cout << "SceneRenderer: More than one camera in provided scene, the first one will be used for rendering." << std::endl;
    }

    auto cameraObj = cameraComponents[0].lock();
    auto cameraComp = cameraObj->getComponent<CameraComponent>().lock();
    _matrixUbo.setView(cameraComp->getViewMatrix());
    _matrixUbo.setProjection(cameraComp->getProjectionMatrix());

    std::vector<LightPtr> lights;
    for (auto it = lightComponents.begin(); it != lightComponents.end(); it++)
    {
        auto lightObj = it->lock();
        auto lightComp = lightObj->getComponent<LightComponent>().lock();
        lights.push_back(lightComp->light);
    }
    sendLightData(lights, cameraComp->camera);

    for (auto it = meshComponents.begin(); it != meshComponents.end(); it++)
    {
        auto meshObj = it->lock();
        unsigned int objId = meshObj->id;
        glm::mat4 modelMatrix = scene.getWorldModelMatrix(objId);
        auto meshComp = meshObj->getComponent<MeshComponent>().lock();
        drawMesh(meshComp->mesh, modelMatrix, cameraComp->getViewMatrix(), meshComp->material, meshComp->shader);
    }
}

void SceneRenderer::sendLightData(std::vector<LightPtr> lights, std::shared_ptr<ViewProjectionProvider> camera)
{
    unsigned int pLightIndex = 0;

    for (auto it = lights.begin(); it != lights.end(); it++)
    {
        switch ((*it)->lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < POINT_LIGHT_MAX_COUNT)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<PointLight> pLight = std::static_pointer_cast<PointLight>(*it);
                    // Transform its world position into view coordinates
                    glm::vec3 lightPos = pLight->getPosition();
                    glm::vec3 viewPos = camera->transformWorldPosition(lightPos);
                    pLight->setPosition(viewPos);
                    // Send it to the UBO
                    _lightUbo.setPoint(pLightIndex++, *pLight);
                }
                break;
        }
    }
    _lightUbo.setPointCount(pLightIndex);
}

void SceneRenderer::drawMesh(MeshPtr mesh, glm::mat4 model, glm::mat4 view, Material material, Shader shader)
{
    shader.use();

    glm::mat4 normal = glm::transpose(glm::inverse(view * model));

    _matrixUbo.setModel(model);
    _matrixUbo.setNormal(normal);

    material.bindTextures();
    shader.setMaterial("material", material);

    mesh->draw();
}
