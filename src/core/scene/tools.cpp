#include "tools.hpp"

#include <memory>

#include "scene_object_component.hpp"
#include "scene_object_component_type.hpp"

#include "components/mesh_component.hpp"
#include "components/light_component.hpp"
#include "components/camera_component.hpp"
#include "components/script_component.hpp"

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
        case SceneObjectComponentType::Script:
             return std::static_pointer_cast<SceneObjectComponent>(cloneComponent<SceneObjectComponentType::Script>(compPtr));
        default:
            return nullptr;
    }
}
