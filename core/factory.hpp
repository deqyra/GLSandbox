#ifndef CORE__FACTORY_HPP
#define CORE__FACTORY_HPP

#include "mesh.hpp"
#include "mesh_generator.hpp"
#include "mesh_generators/mesh_type.hpp"
#include "mesh_generators/type_to_gen_mapping.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"

class Factory
{
    public:
        /// @brief Instantiate and initiliaze a new scene.
        ///
        /// @return Pointer to the instantiated scene.
        static ScenePtr makeScene();

        /// @brief Instantiate and initialize a new scene object.
        ///
        /// @return Pointer to the instantiated scene object.
        static SceneObjectPtr makeSceneObject();

        /// @brief Create a mesh with vertices arranged in a certain shape.
        ///
        /// @tparam T Litteral describing which mesh generator to use.
        ///
        /// @param[in] parameters Parameters to pass to the mesh generator.
        /// Look up generators in core/mesh_generators/ for details.
        ///
        /// @return Pointer to the generated mesh.
        template<MeshType T>
        static MeshPtr makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters);

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Litteral describing which mesh generator to use.
        ///
        /// @param[in] parameters Parameters of the mesh generation.
        /// @param[in] mat Material to render the mesh in.
        /// @param[in] shader Shader to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<MeshType T>
        static SceneObjectPtr makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat = Material(), Shader shader = Shader());
};

template<MeshType T>
MeshPtr Factory::makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters)
{
    using GenType = typename TypeToGenMapping<T>::GenType;
    std::shared_ptr<GenType> gen = std::make_shared<GenType>(parameters);
    return gen->generatePtr();
}

template<MeshType T>
SceneObjectPtr Factory::makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat, Shader shader)
{
    MeshPtr mesh = Factory::makeMesh<T>(parameters);
    SceneObjectPtr obj = Factory::makeSceneObject();
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}

#endif//CORE__FACTORY_HPP