#include "mesh_component.hpp"

using CompType = SceneObjectComponentType;

MeshComponent::MeshComponent(MeshPtr mesh) :
    MeshComponent(mesh, Material(), Shader())
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Material material) :
    MeshComponent(mesh, material, Shader())
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Shader shader) :
    MeshComponent(mesh, Material(), shader)
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Material material, Shader shader) :
    SceneObjectComponent(CompType::Mesh),
    mesh(mesh),
    material(material),
    shader(shader)
{

}

MeshComponent::~MeshComponent()
{

}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<MeshComponent>()
{
    return SceneObjectComponentType::Mesh;
}

template<>
std::string SceneObjectComponent::componentTypeString<MeshComponent>()
{
    return "Mesh";
}
