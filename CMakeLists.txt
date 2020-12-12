####################### 
# CMake configuration #
#######################

cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()



###########
# Toggles #
###########

option(BUILD_STATIC_LIBS "Build libraries as static libraries" OFF)
option(BUILD_RELEASE "Whether to build for a debug or release profile" OFF)

if(${BUILD_STATIC_LIBS})
    set(STATIC_OR_SHARED "STATIC")
else()
    set(STATIC_OR_SHARED "SHARED")
endif()

if (${BUILD_RELEASE})
    set(CMAKE_BUILD_TYPE "Release")
else()
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# if(WIN32)
#     message(STATUS "Windows detected, forcing static libs.")
#     set(BUILD_STATIC_LIBS ON)
# endif()



####################
# Compiler options #
####################

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

# set(CMAKE_CXX_COMPILER_WORKS 1)   # Skips the compiler test, which is platform-dependent



######################
# Project definition #
######################

# Project general info
set(PROJECT_VERSION "1.0")
set(PROJECT_NAME "RenderBoi")

project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "A rendering engine with a modest feature set"
    LANGUAGES C CXX
)

set(PROJECT_MAINTAINER_NAME "François Brachais")
set(PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com")

# GL loader information
set(GL_CONTEXT_VERSION_MAJOR "4")
set(GL_CONTEXT_VERSION_MINOR "2")

# Copyleft notice generation
set(PROJECT_START_YEAR "2019")
set(DATE_SPAN "${PROJECT_START_YEAR}")

string(TIMESTAMP CURRENT_YEAR "%Y")
if(NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR)
    set(DATE_SPAN "${PROJECT_START_YEAR}-${CURRENT_YEAR}")
endif()

set(PROJECT_COPYLEFT "${DATE_SPAN} ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>")

# File configuration
configure_file("${CMAKE_SOURCE_DIR}/renderboi/config/project_macros.config.hpp" "${CMAKE_SOURCE_DIR}/renderboi/project_macros.hpp")



######################
# Build dependencies #
######################

set(EXPORT_LOCATION ${CMAKE_SOURCE_DIR}/external)
set(EXTERNAL_SOURCE_LOCATION ${CMAKE_SOURCE_DIR}/external/source)
set(RENDERBOI_MODULE_LOCATION ${CMAKE_SOURCE_DIR}/renderboi)
file(MAKE_DIRECTORY ${EXPORT_LOCATION})
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/include)
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/lib)

set(CPPTOOLS_LIB_NAME cpptools)
set(GLM_LIB_NAME "glm")
set(GLAD_LIB_NAME "glad")
set(STB_IMAGE_LIB_NAME "stb_image")
set(GLFW3_LIB_NAME "glfw3")
set(RB_WINDOW_LIB_NAME "renderboi.window")
set(RB_CORE_LIB_NAME "renderboi.core")
set(RB_TOOLBOX_LIB_NAME "renderboi.toolbox")
set(RB_EXAMPLES_LIB_NAME "renderboi.examples")

include(ExternalProject)
ExternalProject_Add(${CPPTOOLS_LIB_NAME}_source
    GIT_REPOSITORY https://github.com/deqyra/CppTools
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
    -DCPPTOOLS_EXTERNAL_INSTALL_PREFIX=True
    -DCPPTOOLS_SKIP_TESTS=ON
)

# Build RenderBoi.Examples
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/examples ${CMAKE_BINARY_DIR}/renderboi.examples)
set(RB_EXAMPLES_LIB_NAME renderboi.examples)

# Build executable
add_executable("RenderBoi"
    ${RENDERBOI_MODULE_LOCATION}/main.cpp
    ${RENDERBOI_MODULE_LOCATION}/project_macros.hpp
)   

add_dependencies("RenderBoi"
    "export_${RB_EXAMPLES_LIB_NAME}_lib"
    "export_${RB_EXAMPLES_LIB_NAME}_headers"
)

target_include_directories("RenderBoi" PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories("RenderBoi" PUBLIC ${EXPORT_LOCATION}/lib)

target_link_libraries("RenderBoi" PUBLIC
    ${CPPTOOLS_LIB_NAME}
    ${RB_CORE_LIB_NAME}
    ${RB_WINDOW_LIB_NAME}
    ${RB_TOOLBOX_LIB_NAME}
    ${RB_EXAMPLES_LIB_NAME}
    ${STB_IMAGE_LIB_NAME}
    ${GLFW3_LIB_NAME}
    ${GLAD_LIB_NAME}
) 

# Build and run tests
# enable_testing()
# add_executable(${TEST_TARGET_NAME})
# target_link_libraries(${TEST_TARGET_NAME} PUBLIC "RenderBoi.Core" ${CPPTOOLS_LIB_NAME})
# add_dependencies(${TEST_TARGET_NAME} "copy_assets" "copy_dlls")

# add_test(NAME "tests" COMMAND ${TEST_TARGET_NAME})
# add_custom_command(TARGET ${TEST_TARGET_NAME}
#                    COMMENT "Running tests"
#                    POST_BUILD
#                    COMMAND "./tests")
