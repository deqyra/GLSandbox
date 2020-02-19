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

void MeshDrawer::setEnabled(unsigned int id, bool enabled)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    _meshesEnabled[id] = enabled;
}

bool MeshDrawer::isEnabled(unsigned int id)
{
    if (!hasMesh(id))
    {
        throw std::runtime_error(std::string("MeshDrawer error: no mesh with ID " + std::to_string(id) + ".").c_str());
    }

    return _meshesEnabled[id];
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
    sendLightData(shader);

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

    shader.setMat4f("uProjection", _projection);
    shader.setMat4f("uView", view);
    shader.setMat4f("uModel", model);
    shader.setMat3f("uNormalCorrection", normal);

    shader.setMaterial("uMaterial", mesh->material);

    mesh->setupBuffers();
    mesh->draw();
}

void MeshDrawer::sendLightData(Shader shader)
{

}