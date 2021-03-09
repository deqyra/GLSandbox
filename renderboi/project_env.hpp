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
#define COPYRIGHT_NOTICE "2019-2021 © François Brachais <deqyra@gmail.com>"
#define MIT_LICENSE_NOTICE "This software and its source code are subject to the "\
      "MIT license.\nSee file LICENSE.md at https://github.com/deqyra/RenderBoi. "
#define GL_CONTEXT_VERSION_MAJOR 4
#define GL_CONTEXT_VERSION_MINOR 2

#include <renderboi/window/env_info.hpp>

namespace rb = Renderboi;
namespace rbw = rb::Window;

using AppWindowFactory = rbw::WindowFactory<AppBackend>;

#endif//RENDERBOI__PROJECT_ENV_HPP
