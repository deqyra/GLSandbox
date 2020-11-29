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
        MatrixUBO _matrixUbo;

        /// @brief Handle to a UBO for lights on the GPU.
        LightUBO _lightUbo;

        /// @brief Minimum time interval to keep between rendered frames.
        int64_t _frameIntervalUs;

        /// @brief Last recorded render timestamp. Used to limit the framerate.
        std::chrono::time_point<std::chrono::system_clock> _lastTimestamp;
        
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
        void sendLightData(std::vector<LightPtr>& lights, std::vector<Transform>& worldTransforms, glm::mat4 view);

        /// @brief Issue draw commands for a single mesh.
        /// 
        /// @param meshComponent A pointer to the object whose mesh is to render.
        /// @param viewMatrix The view matrix, provided by the scene camera.
        void drawMesh(SceneObjectPtr meshComponent, glm::mat4 viewMatrix);

    public:
        /// @param framerateLimit How many frames per second the SceneRenderer
        /// should seek to render.
        SceneRenderer(unsigned int framerateLimit = 60);

        /// @brief Render the provided scene.
        ///
        /// @param scene A pointer to the scene which should be rendered.
        ///
        /// @exception If the scene has too many lights of any type for the 
        /// light UBO to handle, the function will throw a std::runtime_error.
        void renderScene(ScenePtr scene);

        /// @brief Set the framerate limit (in frames per second) of the 
        /// renderer.
        ///
        /// @param framerateLimit How many frames per second the SceneRenderer
        /// should seek to render.
        void setFramerateLimit(unsigned int framerateLimit);
};

#endif//RENDERBOI__TOOLBOX__SCENE__SCENE_RENDERER_HPP