#ifndef RENDERBOI__TOOLBOX__SCENE__SCENE_RENDERER_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCENE_RENDERER_HPP

#include <cstdint>
#include <chrono>
#include <memory>
#include <vector>

#include <renderboi/core/lights/light.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/mesh.hpp>
#include <renderboi/core/transform.hpp>
#include <renderboi/core/ubo/matrix_ubo.hpp>
#include <renderboi/core/ubo/light_ubo.hpp>

#include "scene.hpp"
#include "scene_object.hpp"
#include "component.hpp"

/// @brief Manages the render process of a scene.
class SceneRenderer
{
private:
    /// @brief Handle to a UBO for matrices on the GPU.
    mutable MatrixUBO _matrixUbo;

    /// @brief Handle to a UBO for lights on the GPU.
    mutable LightUBO _lightUbo;

    /// @brief Last recorded render timestamp. Used to limit the framerate.
    mutable std::chrono::time_point<std::chrono::system_clock> _lastTimestamp;

    /// @brief Minimum time interval to keep between rendered frames.
    int64_t _frameIntervalUs;
    
    /// @brief Send the scene lights to the GPU.
    ///
    /// @param lights An array filled with pointer to the lights in the 
    /// scene.
    /// @param worldTransforms An array filled with the world transforms of
    /// the lights to send.
    /// @param view The view matrix, provided by the scene camera.
    ///
    /// @exception If there are too many lights of any type and they end up
    /// exceeding the per-type limit defined by the light UBO, the function
    /// will throw a std::runtime_error.
    void sendLightData(
        const std::vector<LightPtr>& lights,
        const std::vector<Transform>& worldTransforms,
        const glm::mat4& view
    ) const;

    /// @brief Issue draw commands for a single mesh.
    /// 
    /// @param meshComponent A pointer to the object whose mesh is to render.
    /// @param viewMatrix The view matrix, provided by the scene camera.
    void drawMesh(const SceneObjectPtr meshComponent, const glm::mat4& viewMatrix) const;

public:
    /// @param framerateLimit How many frames per second the SceneRenderer
    /// should seek to render.
    SceneRenderer(const unsigned int framerateLimit = 60);

    /// @brief Render the provided scene.
    ///
    /// @param scene A pointer to the scene which should be rendered.
    ///
    /// @exception If the scene has too many lights of any type for the 
    /// light UBO to handle, the function will throw a std::runtime_error.
    void renderScene(const ScenePtr scene) const;

    /// @brief Set the framerate limit (in frames per second) of the 
    /// renderer.
    ///
    /// @param framerateLimit How many frames per second the SceneRenderer
    /// should seek to render.
    void setFramerateLimit(const unsigned int framerateLimit);
};

#endif//RENDERBOI__TOOLBOX__SCENE__SCENE_RENDERER_HPP