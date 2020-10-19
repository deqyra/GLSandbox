#ifndef CORE__SCENE__SCENE_RENDERER_HPP
#define CORE__SCENE__SCENE_RENDERER_HPP

#include <cstdint>
#include <chrono>
#include <memory>
#include <vector>

#include "scene.hpp"
#include "scene_object.hpp"
#include "component.hpp"

#include "../lights/light.hpp"
#include "../ubo/matrix_ubo.hpp"
#include "../ubo/light_ubo.hpp"

#include "../mesh.hpp"
#include "../material.hpp"
#include "../shader.hpp"

/// @brief Manages the render process of a scene.
class SceneRenderer
{
    private:
        /// @brief Handle to a UBO for matrices on the GPU.
        MatrixUBO _matrixUbo;

        /// @brief Handle to a UBO for lights on the GPU.
        LightUBO _lightUbo;

        /// @brief Minimum time interval to keep between rendered frames.
        int64_t _intervalUs;

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
        /// @param mesh A pointer to the mesh to render.
        /// @param model The model matrix od the mesh.
        /// @param view The view matrix, provided by the scene camera.
        /// @param mat The material to render the mesh in.
        /// @param shader The shader to render the mesh with.
        void drawMesh(MeshPtr mesh, glm::mat4 model, glm::mat4 view, Material mat, Shader shader);

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

#endif//CORE__SCENE__SCENE_RENDERER_HPP