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
        /// Returns an initialized scene
        static ScenePtr makeScene();

        /// Returns an initialized scene object
        static SceneObjectPtr makeSceneObject();

        /// Returns a mesh whose vertices were generated according to parameters
        ///
        /// @tparam T Litteral describing which type of mesh to generate
        /// @tparam ArgTypes Types of arguments to pass to the chosen mesh generator
        ///
        /// @param[in] args Parameters of the mesh generation
        template<MeshType T>
        static MeshPtr makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters);
};

template<MeshType T>
MeshPtr Factory::makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters)
{
    using GenType = TypeToGenMapping<T>::GenType;
    std::shared_ptr<GenType> gen = std::make_shared<GenType>(parameters);
    return gen->generatePtr();
}

#endif//CORE__FACTORY_HPP