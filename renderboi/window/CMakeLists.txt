cmake_minimum_required( VERSION 3.1...3.15 )

if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
endif(  )

# Compiler options
# C++17 requires at least gcc 7 to be compiled
set( CMAKE_CXX_STANDARD 17 )

if( UNIX )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif( )

# Set up external dependencies
include( ExternalProject )

ExternalProject_Add( "00_glfw3"
    GIT_REPOSITORY https://github.com/glfw/glfw
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
)

# Build window lib
set( EGL_DETECTED "0" )
if( UNIX )
    if ( DEFINED ENV{WAYLAND_DISPLAY} )
        message( STATUS "Wayland detected, disabling GLFW context profile hint for compatibility with EGL." )
        set( EGL_DETECTED "1" )
    endif(  )
endif(  )

configure_file( "${CMAKE_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_SOURCE_DIR}/env_info.hpp" )

add_library( "RenderBoi.Window" ${STATIC_OR_SHARED}
    enums.cpp
    enums.hpp
    env_info.hpp
    gl_window.cpp
    gl_window.hpp
    tools.cpp
    tools.hpp
    window_backend.hpp
    glfw3/glfw3_window_callbacks.cpp
    glfw3/glfw3_window_callbacks.hpp
    glfw3/glfw3_adapter.cpp
    glfw3/glfw3_adapter.hpp
    glfw3/glfw3_window.cpp
    glfw3/glfw3_window.hpp
)
target_include_directories( "RenderBoi.Window" PUBLIC ${INCLUDE_PATH} )
target_link_directories( "RenderBoi.Window" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( "RenderBoi.Window" PUBLIC glfw3 glad ${CMAKE_DL_LIBS} )

# Install
install(
    TARGETS "RenderBoi.Window"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/RenderBoi"
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.hxx"
)