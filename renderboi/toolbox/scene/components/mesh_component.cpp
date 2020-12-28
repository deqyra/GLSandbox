#include "mesh_component.hpp"

#include <renderboi/core/materials.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

MeshComponent::MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh) :
    MeshComponent(sceneObject, mesh, Materials::Default, ShaderBuilder::MinimalShaderProgram())
{

}

MeshComponent::MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh, const Material material) :
    MeshComponent(sceneObject, mesh, material, ShaderBuilder::MinimalShaderProgram())
{

}

MeshComponent::MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh, const ShaderProgram shader) :
    MeshComponent(sceneObject, mesh, Materials::Default, shader)
{

}

MeshComponent::MeshComponent(
    const SceneObjectPtr sceneObject,
    const MeshPtr mesh,
    const Material material,
    const ShaderProgram shader
) :
    Component(ComponentType::Mesh, sceneObject),
    _mesh(mesh),
    _material(material),
    _shader(shader)
{
    if (!mesh)
    {
        throw std::runtime_error("MeshComponent: cannot construct from null mesh pointer.");
    }
}

MeshComponent::~MeshComponent()
{

}

MeshPtr MeshComponent::getMesh() const
{
    return _mesh;
}

void MeshComponent::setMesh(MeshPtr mesh)
{
    if (!mesh)
    {
        throw std::runtime_error("MeshComponent: cannot set mesh pointer to null.");
    }

    _mesh = mesh;
}

Material MeshComponent::getMaterial() const
{
    return _material;
}

void MeshComponent::setMaterial(const Material material)
{
    _material = material;
}

ShaderProgram MeshComponent::getShader() const
{
    return _shader;
}

void MeshComponent::setShader(const ShaderProgram shader)
{
    _shader = shader;
}

MeshComponent* MeshComponent::clone(const SceneObjectPtr newParent) const
{
    // By the time this shared pointer is destroyed (end of scope), responsibility will already have been shared with the cloned MeshComponent
    MeshPtr clonedMesh = std::make_shared<Mesh>(*_mesh);
    // Material and shader clones are automatically copy-constructed by the following call
    return new MeshComponent(newParent, clonedMesh, _material, _shader);
}

template<>
ComponentType Component::componentType<MeshComponent>()
{
    return ComponentType::Mesh;
}

template<>
std::string Component::componentTypeString<MeshComponent>()
{
    return "Mesh";
}

template<>
bool Component::multipleInstancesAllowed<MeshComponent>()
{
    return false;
}
