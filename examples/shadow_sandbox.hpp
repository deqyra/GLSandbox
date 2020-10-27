#ifndef EXAMPLES__SHADOW_SANDBOX_HPP
#define EXAMPLES__SHADOW_SANDBOX_HPP

#include <glm/glm.hpp>

#include "../window/gl_window.hpp"

#include "gl_sandbox.hpp"
#include "../core/camera.hpp"

class ShadowSandbox : public GLSandbox
{
    public:
        static constexpr float LightBaseRange = 30.f;
        static constexpr Camera::CameraParameters CameraParams = {-45.f, -40.f, 1.f};
        static constexpr glm::vec3 StartingCameraPosition = {5.f, 6.f, 5.f};
        static constexpr unsigned int PlaneTileCount = 5;
        static constexpr float PlaneTileSize = 1.f;
        static constexpr float WallSize = PlaneTileCount * PlaneTileSize;
        static constexpr glm::vec3 LightPosition = {WallSize / 2.f, WallSize, WallSize / 2.f};

        // Run the scene in the provided GL window
        virtual void run(GLWindowPtr window);
};

#endif//EXAMPLES__SHADOW_SANDBOX_HPP