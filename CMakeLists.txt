# CMake configuration
cmake_minimum_required( VERSION 3.1...3.15 )

if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
endif(  )

set( CMAKE_CXX_COMPILER_WORKS 1 )   # Skips the compiler test, which is platform-dependent

# Toggles
option( BUILD_STATIC_LIBS "Build libraries as static libraries" OFF )
option( BUILD_RELEASE "Whether to build for a debug or release profile" OFF)

if ( ${BUILD_RELEASE} )
    set( CMAKE_BUILD_TYPE "Release" )
else(  )
    set( CMAKE_BUILD_TYPE "Debug" )
endif(  )

if( WIN32 )
    message( STATUS "Windows detected, forcing static libs." )
    set( BUILD_STATIC_LIBS ON )
endif(  )

if( ${BUILD_STATIC_LIBS} )
    set( STATIC_OR_SHARED "STATIC" )
else(  )
    set( STATIC_OR_SHARED "SHARED" )
endif(  )


# Project general info
set( PROJECT_MAINTAINER_NAME "François Brachais" )
set( PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com" )

# Project library dependency names
set( GLFW3_LIB "glfw" )
set( ASSIMP_LIB "assimp" )
if ( WIN32 )
    set( GLFW3_LIB "glfw/${BUILD_TARGET}/glfw3dll")
endif(  )

# GL loader information
set( GL_CONTEXT_VERSION_MAJOR "4")
set( GL_CONTEXT_VERSION_MINOR "2")

# Header configuration variables
set( PROJECT_VERSION "1.0" )
set( PROJECT_NAME "RenderBoi" )

# Copyleft notice generation
    set( PROJECT_START_YEAR "2019" )
    string( TIMESTAMP CURRENT_YEAR "%Y" )
    # "(C) 2019"
    set( PROJECT_COPYLEFT "Copyleft ${PROJECT_START_YEAR}" )
    if( NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR )
        # "(C) 2019-2020"
        set( PROJECT_COPYLEFT "${PROJECT_COPYLEFT}-${CURRENT_YEAR}" )
    endif( )
    # "(C) 2019-2020 François Brachais <deqyra@gmail.com>"
    set( PROJECT_COPYLEFT "${PROJECT_COPYLEFT} ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>" )

# File configuration
configure_file( "${CMAKE_SOURCE_DIR}/renderboi/config/project_macros.config.hpp" "${CMAKE_SOURCE_DIR}/renderboi/project_macros.hpp" )

# Project definition
project( ${PROJECT_NAME} VERSION ${PROJECT_VERSION}
                                DESCRIPTION "A rendering engine with a modest feature set"
                                LANGUAGES C CXX )

# Compiler options
# C++17 requires at least gcc 7 to be compiled
set( CMAKE_CXX_STANDARD 17 )

if( UNIX )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g" )
endif( )


# Set up external dependencies
set( EXPORT_LOCATION ${CMAKE_BINARY_DIR}/external )
file( MAKE_DIRECTORY ${EXPORT_LOCATION} )
file( MAKE_DIRECTORY ${EXPORT_LOCATION}/include )
file( MAKE_DIRECTORY ${EXPORT_LOCATION}/lib )

include_directories( ${EXPORT_LOCATION}/include )
link_directories( ${EXPORT_LOCATION}/lib )

include( ExternalProject )
ExternalProject_Add( "00cpptools"
    GIT_REPOSITORY https://github.com/deqyra/CppTools
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
    -DCPPTOOLS_EXTERNAL_INSTALL_PREFIX=True
    -DCPPTOOLS_SKIP_TESTS=ON
)

# Build stb_image
add_subdirectory( "external/stb_image" )
add_custom_target( "export_stb_image" )
add_dependencies( "export_stb_image"
    "export_stb_image_lib"
    "export_stb_image_headers"
)
target_include_directories( "stb_image" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "stb_image" PUBLIC ${EXPORT_LOCATION}/lib )

# Build GLAD
add_subdirectory( "external/glad" )
add_custom_target( "export_glad" )
add_dependencies( "export_glad"
    "export_glad_lib"
    "export_glad_headers"
)
target_include_directories( "glad" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "glad" PUBLIC ${EXPORT_LOCATION}/lib )

# Build GLM
add_subdirectory( "external/glm" )
add_custom_target( "export_glm" )
add_dependencies( "export_glm"
    "export_glm_headers"
)

# Build RenderBoi.Window
add_subdirectory( "renderboi/window" )
add_custom_target( "00solve_renderboi.window_dependencies" )
add_dependencies( "00solve_renderboi.window_dependencies"
    "export_glad"
)
add_custom_target( "export_renderboi.window" )
add_dependencies( "export_renderboi.window"
    "00solve_renderboi.window_dependencies"
    "export_renderboi.window_lib"
    "export_renderboi.window_headers"
)
target_include_directories( "renderboi.window" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "renderboi.window" PUBLIC ${EXPORT_LOCATION}/lib )

# Build RenderBoi.Core
add_custom_target( "00solve_renderboi.core_dependencies" )
add_dependencies( "00solve_renderboi.core_dependencies"
    "export_glad"
    "export_glm"
    "export_stb_image"
)
add_subdirectory( "renderboi/core" )
add_custom_target( "export_renderboi.core" )
add_dependencies( "export_renderboi.core"
    "00solve_renderboi.core_dependencies"
    "export_renderboi.core_lib"
    "export_renderboi.core_headers"
)
target_include_directories( "renderboi.core" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "renderboi.core" PUBLIC ${EXPORT_LOCATION}/lib )

# Build RenderBoi.Toolbox
add_subdirectory( "renderboi/toolbox" )
add_custom_target( "00solve_renderboi.toolbox_dependencies" )
add_dependencies( "00solve_renderboi.toolbox_dependencies"
    "export_glm"
    "export_renderboi.core"
    "export_renderboi.window"
)
add_custom_target( "export_renderboi.toolbox" )
add_dependencies( "export_renderboi.toolbox"
    "00solve_renderboi.toolbox_dependencies"
    "export_renderboi.toolbox_lib"
    "export_renderboi.toolbox_headers"
)
target_include_directories( "renderboi.toolbox" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "renderboi.toolbox" PUBLIC ${EXPORT_LOCATION}/lib )

# Build RenderBoi.Examples
add_subdirectory( "renderboi/examples" )
add_custom_target( "00solve_renderboi.examples_dependencies" )
add_dependencies( "00solve_renderboi.examples_dependencies"
    "export_glm"
    "export_renderboi.core"
    "export_renderboi.window"
    "export_renderboi.toolbox"
)
add_custom_target( "export_renderboi.examples" )
add_dependencies( "export_renderboi.examples"
    "00solve_renderboi.examples_dependencies"
    "export_renderboi.examples_lib"
    "export_renderboi.examples_headers"
)
target_include_directories( "renderboi.examples" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "renderboi.examples" PUBLIC ${EXPORT_LOCATION}/lib )

# Build executable
add_executable( "RenderBoi"
                renderboi/main.cpp            renderboi/project_macros.hpp
)   

add_dependencies( "RenderBoi"
    "export_glad"
    "export_renderboi.window"
    "export_renderboi.examples"
    "export_renderboi.core"
)

target_include_directories( "RenderBoi" PUBLIC ${EXPORT_LOCATION}/include )
target_link_directories( "RenderBoi" PUBLIC ${EXPORT_LOCATION}/lib )

target_link_libraries( "RenderBoi" PUBLIC
    cpptools
    renderBoi.core
    renderBoi.window
    renderBoi.toolbox
    renderBoi.examples
    glad
    glfw3
    stb_image
) 

# Build and run tests
# enable_testing( )
# add_executable( ${TEST_TARGET_NAME} )
# target_link_libraries( ${TEST_TARGET_NAME} PUBLIC "RenderBoi.Core" "cpptools" )
# add_dependencies( ${TEST_TARGET_NAME} "copy_assets" "copy_dlls" )

# add_test( NAME "tests" COMMAND ${TEST_TARGET_NAME} )
# add_custom_command( TARGET ${TEST_TARGET_NAME}
#                    COMMENT "Running tests"
#                    POST_BUILD
#                    COMMAND "./tests" )
