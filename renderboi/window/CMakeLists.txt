cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Compiler options
# C++17 requires at least gcc 7 to be compiled
set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

# Set up external dependencies
include(ExternalProject)
ExternalProject_Add(${GLFW3_LIB_NAME}_source
    GIT_REPOSITORY https://github.com/glfw/glfw
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
)

# Build window lib
set(EGL_DETECTED "0")
if(UNIX)
    if (DEFINED ENV{WAYLAND_DISPLAY})
        message(STATUS "Wayland detected, disabling GLFW context profile hint for compatibility with EGL.")
        set(EGL_DETECTED "1")
    endif()
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/env_info.hpp")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build RenderBoi.Window lib
add_library(${RB_WINDOW_LIB_NAME} ${STATIC_OR_SHARED}
    enums.cpp
    enums.hpp
    env_info.hpp
    gl_window.cpp
    gl_window.hpp
    input_processor.cpp
    input_processor.hpp
    window_factory.cpp
    window_factory.hpp
    window_backend.hpp
    glfw3/glfw3_window_callbacks.cpp
    glfw3/glfw3_window_callbacks.hpp
    glfw3/glfw3_adapter.cpp
    glfw3/glfw3_adapter.hpp
    glfw3/glfw3_window.cpp
    glfw3/glfw3_window.hpp
)
add_dependencies(${RB_WINDOW_LIB_NAME}
    "${GLFW3_LIB_NAME}_source"
    "export_${GLAD_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_headers"
)
target_include_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_WINDOW_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${GLFW3_LIB_NAME}
    ${GLAD_LIB_NAME}
    pthread
)

add_custom_target("export_${RB_WINDOW_LIB_NAME}_lib")
add_dependencies("export_${RB_WINDOW_LIB_NAME}_lib" ${RB_WINDOW_LIB_NAME})
add_custom_command(
  TARGET "export_${RB_WINDOW_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_WINDOW_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target("export_${RB_WINDOW_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${RB_WINDOW_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
)
