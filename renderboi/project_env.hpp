/*     // \\
      //   \\
     //     \\
    //WARNING\\
   //         \\
   =============
   
   DO NOT edit project_env.hpp, or your changes
   will be OVERWRITTEN at the next run of CMake.
   Instead, edit config/project_env.config.hpp.
*/

#ifndef RENDERBOI__PROJECT_ENV_HPP
#define RENDERBOI__PROJECT_ENV_HPP

#define PROJECT_NAME "RenderBoi"
#define PROJECT_VERSION "1.0"
#define COPYLEFT_NOTICE "2019-2021 François Brachais <deqyra@gmail.com>"
#define GL_CONTEXT_VERSION_MAJOR 4
#define GL_CONTEXT_VERSION_MINOR 2

#include <renderboi/window/glfw3/glfw3_window_factory.hpp>
#include <renderboi/window/glfw3/glfw3_window_callbacks.hpp>

namespace rb = Renderboi;

static constexpr rb::WindowBackend UsedBackend = rb::WindowBackend::GLFW3;
using AppWindowFactory = rb::WindowFactory<UsedBackend>;

#endif//RENDERBOI__PROJECT_ENV_HPP
