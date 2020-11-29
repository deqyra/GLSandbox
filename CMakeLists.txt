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

# Copy assets folder
add_custom_target( "copy_assets" )
add_custom_command( TARGET "copy_assets"
                    COMMENT "Copying assets"
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ./assets )

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
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif( )


# Set up external dependencies
set( CMAKE_INSTALL_PREFIX ${EXTERNAL_INSTALL_LOCATION} )
set( RENDERBOI_INCLUDE_DIR ${EXTERNAL_INSTALL_LOCATION} )
set( EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external )
file( MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION} )

include( ExternalProject )

ExternalProject_Add( "00_CppTools"
    GIT_REPOSITORY https://github.com/deqyra/CppTools
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DCPPTOOLS_EXTERNAL_INSTALL_PREFIX=True
    -DCPPTOOLS_SKIP_TESTS=ON
)

ExternalProject_Add( "01_STBImage"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/stb_image"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
)

ExternalProject_Add( "02_GLAD"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/glad"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
)

ExternalProject_Add( "03_RenderBoiWindow"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/renderboi/window"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
    -DINCLUDE_PATH=${EXTERNAL_INSTALL_LOCATION}/include
)

ExternalProject_Add( "04_GLM"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/glm"
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
)

ExternalProject_Add( "05_RenderBoiCore"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/renderboi/core"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
    -DINCLUDE_PATH=${EXTERNAL_INSTALL_LOCATION}/include
)

ExternalProject_Add( "06_RenderBoiToolbox"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/renderboi/toolbox"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
    -DINCLUDE_PATH=${EXTERNAL_INSTALL_LOCATION}/include
)

ExternalProject_Add( "07_RenderBoiExamples"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/renderboi/examples"
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    -DSTATIC_OR_SHARED=${STATIC_OR_SHARED}
    -DINCLUDE_PATH=${EXTERNAL_INSTALL_LOCATION}/include
)

# Build executable
add_executable( "RenderBoi"
                renderboi/main.cpp            renderboi/project_macros.hpp )   
target_include_directories( "RenderBoi" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include )
target_link_directories( "RenderBoi" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( "RenderBoi" PUBLIC
                        cpptools
                        RenderBoi.Core
                        RenderBoi.Window
                        RenderBoi.Toolbox
                        RenderBoi.Examples
                        glad
                        glfw3
                        stb_image
                    ) 
add_dependencies( "RenderBoi" "copy_assets" )
if( WIN32 )
    #add_dependencies( "RenderBoi" "copy_dlls" )
endif(  )

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
