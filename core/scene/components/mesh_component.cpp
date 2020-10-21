#include "mesh_component.hpp"

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh) :
    MeshComponent(sceneObject, mesh, Material(), Shader())
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material) :
    MeshComponent(sceneObject, mesh, material, Shader())
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Shader shader) :
    MeshComponent(sceneObject, mesh, Material(), shader)
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material, Shader shader) :
    Component(ComponentType::Mesh, sceneObject),
    mesh(mesh),
    material(material),
    shader(shader)
{

}

MeshComponent::~MeshComponent()
{

}

MeshComponent* MeshComponent::clone(SceneObjectPtr newParent)
{
    // By the time this shared pointer is destroyed (end of scope), responsibility will already have been shared with the cloned MeshComponent
    MeshPtr clonedMesh = std::make_shared<Mesh>(*mesh);
    // Material and shader clones are automatically copy-constructed by the following call
    return new MeshComponent(newParent, clonedMesh, material, shader);
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
