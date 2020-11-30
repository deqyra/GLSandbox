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

ExternalProject_Add( "00glfw3"
    GIT_REPOSITORY https://github.com/glfw/glfw
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
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

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/env_info.hpp" )

set( LIBNAME "renderboi.window")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${LIBNAME}_headers.cmake
    "file( COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.h\"
        PATTERN \"*.hpp\"
        PATTERN \"*.hxx\"
    )\n"
)

# Build RenderBoi.Window lib
add_library( ${LIBNAME} ${STATIC_OR_SHARED}
    enums.cpp
    enums.hpp
    env_info.hpp
    gl_window.cpp
    gl_window.hpp
    input_processor.cpp
    input_processor.hpp
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
add_dependencies( ${LIBNAME} "00glfw3" )
target_include_directories( ${LIBNAME} PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( ${LIBNAME} PUBLIC ${EXPORT_LOCATION}/lib )
target_link_libraries( ${LIBNAME} PUBLIC glfw3 glad ${CMAKE_DL_LIBS} )

add_custom_target( "export_${LIBNAME}_lib" )
add_dependencies( "export_${LIBNAME}_lib" ${LIBNAME})
add_custom_command(
  TARGET "export_${LIBNAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBNAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target( "export_${LIBNAME}_headers" )
add_custom_command(
  TARGET "export_${LIBNAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${LIBNAME}_headers.cmake
)
