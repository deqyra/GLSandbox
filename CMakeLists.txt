###############################################################################
#                                                                             #
#                             CMake configuration                             #
#                                                                             #
###############################################################################

cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()



###############################################################################
#                                                                             #
#                                   Toggles                                   #
#                                                                             #
###############################################################################

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



###############################################################################
#                                                                             #
#                              Compiler options                               #
#                                                                             #
###############################################################################

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

# set(CMAKE_CXX_COMPILER_WORKS 1)
# Skips the compiler test, which is platform-dependent



###############################################################################
#                                                                             #
#                             Project definition                              #
#                                                                             #
###############################################################################

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



###############################################################################
#                                                                             #
#                            Dependency locations                             #
#                                                                             #
###############################################################################

set(EXPORT_LOCATION ${CMAKE_SOURCE_DIR}/external)
set(EXTERNAL_SOURCE_LOCATION ${CMAKE_SOURCE_DIR}/external/source)
set(RENDERBOI_MODULE_LOCATION ${CMAKE_SOURCE_DIR}/renderboi)
file(MAKE_DIRECTORY ${EXPORT_LOCATION})
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/include)
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/lib)



###############################################################################
#                                                                             #
#                  Target names and file names masterlists                    #
#                                                                             #
###############################################################################
#    (easier to browse here than to go and update it in each subdirectory)    #
###############################################################################

set(CPPTOOLS_LIB_NAME "cpptools")
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

set(RB_CORE_SOURCE_FILES
    camera.cpp
    camera.hpp
    material.cpp
    material.hpp
    materials.hpp
    mesh.cpp
    mesh.hpp
    pixel_space.hpp
    transform.cpp
    transform.hpp
    shader/shader.cpp
    shader/shader.hpp
    shader/shader_builder.cpp
    shader/shader_builder.hpp
    shader/shader_config.cpp
    shader/shader_config.hpp
    shader/shader_feature.cpp
    shader/shader_feature.hpp
    shader/shader_info.cpp
    shader/shader_info.hpp
    shader/shader_program.cpp
    shader/shader_program.hpp
    shader/shader_stage.cpp
    shader/shader_stage.hpp
    texture_2d.cpp
    texture_2d.hpp
    vertex.hpp
    lights/directional_light.cpp
    lights/directional_light.hpp
    lights/light.cpp
    lights/light.hpp
    lights/light_type.hpp
    lights/point_light.cpp
    lights/point_light.hpp
    lights/spot_light.cpp
    lights/spot_light.hpp 
    lights/tools.cpp
    lights/tools.hpp 
    ubo/ubo_info.hpp 
    ubo/uniform_buffer_object.cpp
    ubo/uniform_buffer_object.hpp 
    ubo/light_ubo.cpp
    ubo/light_ubo.hpp 
    ubo/matrix_ubo.cpp
    ubo/matrix_ubo.hpp 
)

set(RB_WINDOW_SOURCE_FILES
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

set(RB_TOOLBOX_SOURCE_FILES
    common_macros.hpp
    factory.cpp
    factory.hpp
    mesh_generator.hpp
    input_processing_script.cpp
    input_processing_script.hpp 
    script.cpp
    script.hpp 
    mesh_generators/all_mesh_generators.hpp
    mesh_generators/axes_generator.cpp
    mesh_generators/axes_generator.hpp
    mesh_generators/cube_generator.cpp
    mesh_generators/cube_generator.hpp
    mesh_generators/plane_generator.cpp
    mesh_generators/plane_generator.hpp
    mesh_generators/tetrahedron_generator.cpp
    mesh_generators/tetrahedron_generator.hpp
    mesh_generators/torus_generator.cpp
    mesh_generators/torus_generator.hpp
    mesh_generators/mesh_type.hpp
    mesh_generators/type_to_gen_mapping.hpp
    scene/component.cpp
    scene/component.hpp 
    scene/component_type.hpp 
    scene/object_transform.cpp
    scene/object_transform.hpp
    scene/scene.cpp
    scene/scene.hpp 
    scene/scene_object_metadata.hpp 
    scene/scene_object.cpp
    scene/scene_object.hpp 
    scene/scene_renderer.cpp
    scene/scene_renderer.hpp 
    scene/components/all_components.hpp 
    scene/components/camera_component.cpp
    scene/components/camera_component.hpp 
    scene/components/input_processing_script_component.cpp
    scene/components/input_processing_script_component.hpp 
    scene/components/light_component.cpp
    scene/components/light_component.hpp 
    scene/components/mesh_component.cpp
    scene/components/mesh_component.hpp 
    scene/components/script_component.cpp
    scene/components/script_component.hpp 
    scripts/basic_input_manager.cpp
    scripts/basic_input_manager.hpp 
    scripts/mouse_camera_script.cpp
    scripts/mouse_camera_script.hpp 
    scripts/camera_aspect_ratio_script.cpp
    scripts/camera_aspect_ratio_script.hpp 
    scripts/keyboard_movement_script.cpp
    scripts/keyboard_movement_script.hpp 
    scripts/lambda_script.cpp
    scripts/lambda_script.hpp 
)

set(RB_EXAMPLES_SOURCE_FILES
    gl_sandbox.hpp
    lighting_sandbox.cpp
    lighting_sandbox.hpp
    shadow_sandbox.cpp
    shadow_sandbox.hpp
)



###############################################################################
#                                                                             #
#                         Include direct dependencies                         #
#                                                                             #
###############################################################################

# Build RenderBoi.Examples
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/examples ${CMAKE_BINARY_DIR}/${RB_EXAMPLES_LIB_NAME})



###############################################################################
#                                                                             #
#                         Build RenderBoi executable                          #
#                                                                             #
###############################################################################

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
    ${RB_WINDOW_LIB_NAME}
    ${RB_EXAMPLES_LIB_NAME}
) 



###############################################################################
#                                                                             #
#                                    Tests                                    #
#                                                                             #
###############################################################################

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
