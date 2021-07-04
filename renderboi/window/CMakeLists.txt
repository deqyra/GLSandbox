# Detect EGL platform
set(EGL_DETECTED 0)
if(UNIX)
    if (DEFINED ENV{WAYLAND_DISPLAY})
        set(EGL_DETECTED 1)
        message(STATUS "Wayland detected, EGL compatibility enabled.")
    endif()
endif()

# Detect which window backend to use
set(WINDOW_BACKEND_NAME "Unknown")
set(WINDOW_BACKEND_LIB_NAME "")
set(NO_WINDOW_BACKEND 0)

set(USE_GLFW3 0)

if(WINDOW_BACKEND_GLFW3)
    set(WINDOW_BACKEND_NAME "GLFW3")
    set(WINDOW_BACKEND_LIB_NAME "glfw3")
    set(USE_GLFW3 1)

    # Set up external dependencies
    include(ExternalProject)
    ExternalProject_Add(window_backend_source
        GIT_REPOSITORY https://github.com/glfw/glfw
        UPDATE_DISCONNECTED TRUE
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
    )
else()
    add_custom_target("window_backend_source")
    add_custom_command(
        TARGET "window_backend_source"
        COMMAND ${CMAKE_COMMAND} -E echo "No window backend was selected! Building the project will fail..."
    )
    set(NO_WINDOW_BACKEND 1)
endif()

# Setup flags accordingly
if(USE_GLFW3)
    ####################################################
    # Which policy to use for borderless windowed mode #
    ####################################################

    # GLFW does not provide a way to get the desktop video mode of a monitor,
    # which is needed in order to allow for "borderless windowed" mode.
    #
    # RenderBoi will try to do this in one of two ways:
    # - Native mode policy: RenderBoi remembers the current video modes of all
    # monitors when it first starts up, and uses those to go borderless windowed
    # on a given monitor.
    # - Max mode policy: RenderBoi assumes the desktop video mode of a monitor
    # simply is the "largest" available video mode of that monitor, and will use
    # it to go borderless windowed on that monitor.
    #
    # Unfortunately, neither of these methods guarantees that the resulting
    # video mode actually is the desktop video mode of any given monitor.
    #
    # The default policy for going borderless windowed is the native mode 
    # policy. Provide the following at configure time to change the policy:
    # -DGLFW3_BORDERLESS_POLICY_MAX_MODE=ON
    # The variables desclared hereafter will be used to configure the header
    # file ./config/env_info.config.hpp.

    # Default policy is native mode policy
    set(GLFW3_BORDERLESS_POLICY_NATIVE_MODE_VAL 1)
    set(GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL 0)

    # If told otherwise...
    if(GLFW3_BORDERLESS_POLICY_MAX_MODE AND NOT GLFW3_BORDERLESS_POLICY_NATIVE_MODE)
        # Set options accordingly
        set(GLFW3_BORDERLESS_POLICY_NATIVE_MODE OFF)
        set(GLFW3_BORDERLESS_POLICY_NATIVE_MODE_VAL 0)
        set(GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL 1)
    endif()
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/env_info.hpp")

# Generate script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build window lib
add_library(${RB_WINDOW_LIB_NAME} ${STATIC_OR_SHARED}
    ${RB_WINDOW_SOURCE_FILES}
)
add_dependencies(${RB_WINDOW_LIB_NAME}
    "window_backend_source"
    "export_${CPPTOOLS_LIB_NAME}_headers"
    "export_${CPPTOOLS_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_headers"
    "export_${RB_UTILITIES_LIB_NAME}_lib"
    "export_${RB_UTILITIES_LIB_NAME}_headers"
)
target_include_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)

set(THREADING_LIB "")
if (UNIX)
    set(THREADING_LIB "pthread")
endif()

target_link_libraries(${RB_WINDOW_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${WINDOW_BACKEND_LIB_NAME}
    ${THREADING_LIB}
    ${GLAD_LIB_NAME}
    ${RB_UTILITIES_LIB_NAME}
)

# Custom target to export window lib
add_custom_target("export_${RB_WINDOW_LIB_NAME}_lib")
add_dependencies("export_${RB_WINDOW_LIB_NAME}_lib" ${RB_WINDOW_LIB_NAME})
add_custom_command(
    TARGET "export_${RB_WINDOW_LIB_NAME}_lib"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_WINDOW_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

# Custom target to export window headers
add_custom_target("export_${RB_WINDOW_LIB_NAME}_headers")
add_custom_command(
    TARGET "export_${RB_WINDOW_LIB_NAME}_headers"
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
)
