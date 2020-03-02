#include "tools.hpp"

#include <memory>

#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"

std::shared_ptr<SceneObjectComponent> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr)
{
    switch (compPtr->type)
    {
        case SceneObjectComponentType::Mesh:
             return std::static_pointer_cast<SceneObjectComponent>(cloneComponent<SceneObjectComponentType::Mesh>(compPtr));
        case SceneObjectComponentType::Light:
             return std::static_pointer_cast<SceneObjectComponent>(cloneComponent<SceneObjectComponentType::Light>(compPtr));
        case SceneObjectComponentType::Camera:
             return std::static_pointer_cast<SceneObjectComponent>(cloneComponent<SceneObjectComponentType::Camera>(compPtr));
        default:
            return nullptr;
    }
}

template<SceneObjectComponentType T>
std::shared_ptr<typename TypeFromEnum<T>::type> cloneComponent(std::shared_ptr<SceneObjectComponent> compPtr)
{
    using CompRealType = typename TypeFromEnum<T>::type;
    using CompPtr = std::shared_ptr<CompRealType>;

    CompPtr realTypePtr = std::static_pointer_cast<CompRealType>(compPtr);
    // Clone the component on the heap and return a pointer to it
    realTypePtr.reset(new CompRealType(*realTypePtr));

    return realTypePtr;
}
