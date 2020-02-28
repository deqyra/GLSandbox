#include "mesh_component.hpp"

using CompType = SceneObjectComponentType;

MeshComponent::MeshComponent(Mesh mesh) :
    MeshComponent(mesh, Material(), Shader())
{

}

MeshComponent::MeshComponent(Mesh mesh, Material material) :
    MeshComponent(mesh, material, Shader())
{

}

MeshComponent::MeshComponent(Mesh mesh, Shader shader) :
    MeshComponent(mesh, Material(), shader)
{

}

MeshComponent::MeshComponent(Mesh mesh, Material material, Shader shader) :
    SceneObjectComponent(CompType::Mesh),
    mesh(mesh),
    material(material),
    shader(shader)
{

}

template<>
SceneObjectComponentType SceneObjectComponent::componentType<MeshComponent>()
{
    return CompType::Mesh;
}
