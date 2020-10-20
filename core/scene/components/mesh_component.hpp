#ifndef CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../mesh.hpp"
#include "../../material.hpp"
#include "../../shader.hpp"

#include <string>

/// @brief Component allowing to attach a mesh to a scene object.
class MeshComponent : public Component
{
    private:
        MeshComponent(MeshComponent& other) = delete;
        MeshComponent& operator=(const MeshComponent& other) = delete;

    public:
        /// @param mesh Pointer to the mesh which the component should use.
        MeshComponent(MeshPtr mesh);

        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        MeshComponent(MeshPtr mesh, Material material);

        /// @param mesh Pointer to the mesh which the component should use.
        /// @param shader Shader which the mesh should be rendred by.
        MeshComponent(MeshPtr mesh, Shader shader);

        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        /// @param shader Shader which the mesh should be rendred by.
        MeshComponent(MeshPtr mesh, Material material, Shader shader);

        virtual ~MeshComponent();

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual MeshComponent* clone();

        /// @brief Pointer to the mesh data of the object.
        MeshPtr mesh;

        /// @brief Material to paint the mesh with.
        Material material;
        
        /// @brief Shader to render the mesh with.
        Shader shader;
};

template<>
ComponentType Component::componentType<MeshComponent>();

template<>
std::string Component::componentTypeString<MeshComponent>();

template<>
bool Component::multipleInstancesAllowed<MeshComponent>();

#endif//CORE__SCENE__COMPONENTS__MESH_COMPONENT_HPP