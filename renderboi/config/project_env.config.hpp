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

#define PROJECT_NAME "@PROJECT_NAME@"
#define PROJECT_VERSION "@PROJECT_VERSION@"
#define COPYRIGHT_NOTICE "@PROJECT_COPYRIGHT@"
#define MIT_LICENSE_NOTICE "This software and its source code are subject to the "\
      "MIT license.\nSee file LICENSE.md at https://github.com/deqyra/RenderBoi. "
#define GL_CONTEXT_VERSION_MAJOR @GL_CONTEXT_VERSION_MAJOR@
#define GL_CONTEXT_VERSION_MINOR @GL_CONTEXT_VERSION_MINOR@

#include <renderboi/window/glfw3/glfw3_window_factory.hpp>
#include <renderboi/window/glfw3/glfw3_utilities.hpp>

namespace rb = Renderboi;

static constexpr rb::WindowBackend UsedBackend = rb::WindowBackend::GLFW3;
using AppWindowFactory = rb::WindowFactory<UsedBackend>;

#endif//RENDERBOI__PROJECT_ENV_HPP