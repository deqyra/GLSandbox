/**
    GLTest, mesh_drawer.cpp
    Purpose: Implement class MeshDrawer. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 13/02/2020
 */
#include <memory>
#include <unordered_map>
#include <vector>

#include "mesh_drawer.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "view_provider.hpp"

MeshDrawer::MeshDrawer(std::shared_ptr<ViewProjectionProvider> camera) :
    _camera(camera),
    _matrixUbo(),
    _lightUbo()
{
    _matrixUbo.setView(camera->getViewMatrix());
    _matrixUbo.setProjection(camera->getProjectionMatrix());
}

void MeshDrawer::registerMesh(MeshPtr mesh, Shader shader)
{
    unsigned int id = mesh->id;
    if (hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: mesh with ID " + std::to_string(id) + " already exists.").c_str());
    }

    _meshes[id] = mesh;
    _shaders[id] = shader;
    _meshesEnabled[id] = true;
}

std::shared_ptr<Mesh> MeshDrawer::getMesh(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }
    return _meshes[id];
}

bool MeshDrawer::hasMesh(unsigned int id)
{
    auto it = _meshes.find(id);
    return it != _meshes.end();
}

void MeshDrawer::removeMesh(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _meshes.erase(id);
    _shaders.erase(id);
    _meshesEnabled.erase(id);
}

Shader MeshDrawer::getShader(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    return _shaders[id];
}

void MeshDrawer::setShader(unsigned int id, Shader shader)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _shaders[id] = shader;
}

void MeshDrawer::enableMesh(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _meshesEnabled[id] = true;
}

void MeshDrawer::disableMesh(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _meshesEnabled[id] = false;
}

void MeshDrawer::setMeshEnabled(unsigned int id, bool enabled)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _meshesEnabled[id] = enabled;
}

bool MeshDrawer::isMeshEnabled(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    return _meshesEnabled[id];
}

void MeshDrawer::registerLight(LightPtr light)
{
    unsigned int id = light->id;
    if (hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: light with ID " + std::to_string(id) + " already exists.").c_str());
    }

    _lights[id] = light;
    _lightsEnabled[id] = true;
}

std::shared_ptr<Light> MeshDrawer::getLight(unsigned int id)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }
    return _lights[id];
}

bool MeshDrawer::hasLight(unsigned int id)
{
    auto it = _lights.find(id);
    return it != _lights.end();
}

void MeshDrawer::removeLight(unsigned int id)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }

    _lights.erase(id);
    _lightsEnabled.erase(id);
}

void MeshDrawer::enableLight(unsigned int id)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }

    _lightsEnabled[id] = true;
}

void MeshDrawer::disableLight(unsigned int id)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }

    _lightsEnabled[id] = false;
}

void MeshDrawer::setLightEnabled(unsigned int id, bool enabled)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }

    _lightsEnabled[id] = enabled;
}

bool MeshDrawer::isLightEnabled(unsigned int id)
{
    if (!hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no light with ID " + std::to_string(id) + ".").c_str());
    }

    return _lightsEnabled[id];
}

void MeshDrawer::setCamera(std::shared_ptr<ViewProjectionProvider> camera)
{
    _camera = camera;
}

std::shared_ptr<ViewProjectionProvider> MeshDrawer::getCamera()
{
    return _camera;
}

void MeshDrawer::renderFrame()
{
    glm::mat4 view = _camera->getViewMatrix();
    _matrixUbo.setView(view);

    sendLightData();

    for (auto it = _meshes.begin(); it != _meshes.end(); it++)
    {
        drawMesh(it->first);
    }
}

void MeshDrawer::drawMesh(unsigned int id)
{
    if (!_meshesEnabled[id]) return;

    Shader shader = _shaders[id];
    shader.use();

    glm::mat4 view = _camera->getViewMatrix();

    std::shared_ptr<Mesh> mesh = _meshes[id];
    glm::mat4 model = mesh->getModelMatrix();
    
    glm::mat4 normal = glm::transpose(glm::inverse(view * model));

    _matrixUbo.setModel(model);
    _matrixUbo.setNormal(normal);

    mesh->material.bindTextures();
    shader.setMaterial("material", mesh->material);

    mesh->setupBuffers();
    mesh->draw();
}

void MeshDrawer::sendLightData()
{
    unsigned int pLightIndex = 0;

    glm::mat4 view = _camera->getViewMatrix();

    for (auto it = _lights.begin(); it != _lights.end(); it++)
    {
        std::shared_ptr<Light> light = it->second;
        if (!_lightsEnabled[light->id]) continue;

        switch (light->lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < POINT_LIGHT_MAX_COUNT)
                {
                    // Retrieve the light as a PointLight
                    std::shared_ptr<PointLight> pLight = std::static_pointer_cast<PointLight>(light);
                    PointLight tmpLight = (*pLight);
                    // Transform its world position into view coordinates
                    glm::vec3 tmpPos = tmpLight.getPosition();
                    glm::vec3 viewPos = _camera->transformWorldPosition(tmpPos);
                    tmpLight.setPosition(viewPos);
                    // Send it to the UBO
                    _lightUbo.setPoint(pLightIndex++, tmpLight);
                }
                break;
        }
    }
    _lightUbo.setPointCount(pLightIndex);
}
