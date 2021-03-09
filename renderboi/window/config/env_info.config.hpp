/*     // \\
      //   \\
     //     \\
    //WARNING\\
   //         \\
   =============
   
   DO NOT edit window/env_info.hpp, or your changes
   will be OVERWRITTEN at the next run of CMake.
   Instead, edit window/config/env_info.config.hpp.
*/

#ifndef RENDERBOI__WINDOW__ENV_INFO_HPP
#define RENDERBOI__WINDOW__ENV_INFO_HPP

#if @EGL_DETECTED@
#define EGL_DETECTED
#endif

#if @NO_WINDOW_BACKEND@
#error No window backend was selected for compilation. Cannot build window module.
#endif

namespace rb = Renderboi;
namespace rbw = rb::Window;

#if @USE_GLFW3@
      #define GLFW3_BORDERLESS_POLICY_NATIVE

      #if (@GLFW3_BORDERLESS_POLICY_NATIVE_VAL@ != 1) && (@GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL@ == 1)
            #undef GLFW3_BORDERLESS_POLICY_NATIVE
            #define GLFW3_BORDERLESS_POLICY_MAX_MODE
      #endif

      #include <renderboi/window/glfw3/glfw3_window_factory.hpp>
      #include <renderboi/window/glfw3/glfw3_utilities.hpp>
            
      static const void* AppWindowErrorCallback = (void*)(&rbw::GLFW3Utilities::globalGlfwErrorCallback);
#endif

#include "window_backend.hpp"

static constexpr rbw::WindowBackend AppBackend = rbw::WindowBackend::@WINDOW_BACKEND_NAME@;

#endif//RENDERBOI__WINDOW__ENV_INFO_HPP