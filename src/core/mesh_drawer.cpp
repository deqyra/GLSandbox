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

MeshDrawer::MeshDrawer() :
    _camera(nullptr),
    _projection(glm::mat4(1.f)),
    _lightsSetup(false)
{

}

MeshDrawer::MeshDrawer(std::shared_ptr<ViewProvider> camera, glm::mat4 projection) :
    _camera(camera),
    _projection(projection),
    _lightsSetup(false)
{

}

void MeshDrawer::registerMesh(Mesh* mesh, Shader shader)
{
    unsigned int id = mesh->id;
    if (hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: mesh with ID " + std::to_string(id) + " already exists.").c_str());
    }

    _meshes[id] = std::shared_ptr<Mesh>(mesh);
    _shaders[id] = shader;
    _meshesEnabled[id] = true;
}

void MeshDrawer::registerMesh(std::shared_ptr<Mesh> mesh, Shader shader)
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

void MeshDrawer::registerLight(Light* light)
{
    unsigned int id = light->id;
    if (hasLight(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: light with ID " + std::to_string(id) + " already exists.").c_str());
    }

    _lights[id] = std::shared_ptr<Light>(light);
    _lightsEnabled[id] = true;
}

void MeshDrawer::registerLight(std::shared_ptr<Light> light)
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

void MeshDrawer::setCamera(ViewProvider* camera)
{
    _camera.reset(camera);
}

void MeshDrawer::setCamera(std::shared_ptr<ViewProvider> camera)
{
    _camera = camera;
}

std::shared_ptr<ViewProvider> MeshDrawer::getCamera()
{
    return _camera;
}

glm::mat4 MeshDrawer::getProjection()
{
    return _projection;
}

void MeshDrawer::setProjection(glm::mat4 projection)
{
    _projection = projection;
}

void MeshDrawer::drawMesh(unsigned int id)
{
    if (_camera == nullptr)
    {
        throw std::runtime_error("MeshDrawer error: drawMesh was called with no camera set.");
    }

    if (_projection == glm::mat4(1.f))
    {
        throw std::runtime_error("MeshDrawer error: drawMesh was called with no projection matrix set.");
    }

    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    drawMeshUnsafe(id);
}

void MeshDrawer::drawMeshes()
{
    if (_camera == nullptr)
    {
        throw std::runtime_error("MeshDrawer error: drawMeshes was called with no camera set.");
    }

    if (_projection == glm::mat4(1.f))
    {
        throw std::runtime_error("MeshDrawer error: drawMeshes was called with no projection matrix set.");
    }

    for (auto it = _meshes.begin(); it != _meshes.end(); it++)
    {
        drawMeshUnsafe(it->first);
    }
}

void MeshDrawer::drawMeshUnsafe(unsigned int id)
{
    if (!_meshesEnabled[id]) return;

    Shader shader = _shaders[id];
    shader.use();
    sendAllLightData(shader);

    glm::mat4 view = _camera->getViewMatrix();

    std::shared_ptr<Mesh> mesh = _meshes[id];
    glm::mat4 model = mesh->getModelMatrix();
    
    glm::mat3 normal = glm::mat3(0.f);
    glm::mat4 normal4 = glm::transpose(glm::inverse(view * model));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            normal[i][j] = normal4[i][j];
        }
    }

    typedef UniformDestination UD;
    shader.setMat4f(UD::ProjectionMatrix, _projection);
    shader.setMat4f(UD::ViewMatrix, view);
    shader.setMat4f(UD::ModelMatrix, model);
    shader.setMat3f(UD::NormalMatrix, normal);

    shader.setMaterial(UD::Material, mesh->material);

    mesh->setupBuffers();
    mesh->draw();
}

void MeshDrawer::sendAllLightData(Shader& shader)
{
    typedef UniformDestination UD;
    typedef ShaderConstant SC;

    unsigned int maxPLightCount = (unsigned int) shader.getConstant(SC::MaxPointLightCount);
    unsigned int pLightIndex = 0;

    glm::mat4 view = _camera->getViewMatrix();

    for (auto it = _lights.begin(); it != _lights.end(); it++)
    {
        std::shared_ptr<Light> light = it->second;
        if (!_lightsEnabled[light->id]) continue;

        switch (light->lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < maxPLightCount)
                {
                    std::shared_ptr<PointLight> pLight = std::static_pointer_cast<PointLight>(light);
                    PointLight viewLight = (*pLight);
                    glm::vec3 wPos = viewLight.getPosition();
                    glm::vec4 vPos = view * glm::vec4(wPos, 1.f);
                    viewLight.setPosition(glm::vec3(vPos));
                    shader.setPointLightArray(UD::PointLightArray, pLightIndex++, viewLight);
                }
                break;
        }
    }
    shader.setUint(UD::PointLightCount, pLightIndex);
}
