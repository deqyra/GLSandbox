#ifndef CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../mesh.hpp"
#include "../../material.hpp"
#include "../../shader.hpp"

#include <string>

// Component derived class representing the mesh of a scene object
class MeshComponent : public Component
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Components are meant to be used only through pointers
        MeshComponent(MeshComponent& other) = delete;
        MeshComponent& operator=(const MeshComponent& other) = delete;

    public:
        MeshComponent(MeshPtr mesh);
        MeshComponent(MeshPtr mesh, Material material);
        MeshComponent(MeshPtr mesh, Shader shader);
        MeshComponent(MeshPtr mesh, Material material, Shader shader);

        virtual ~MeshComponent();

        // Get a raw pointer to a new MeshComponent instance cloned from this. The inner light is cloned as well. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual MeshComponent* clone();

        // Pointer to the mesh data of the object
        MeshPtr mesh;
        // Material to paint on the mesh
        Material material;
        // Shader to use to render the mesh
        Shader shader;
};

template<>
ComponentType Component::componentType<MeshComponent>();

template<>
std::string Component::componentTypeString<MeshComponent>();

#endif//CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP