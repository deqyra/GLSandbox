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

if(${BUILD_STATIC_LIBS} OR WIN32)
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

set(CMAKE_CXX_STANDARD 20)

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
configure_file("${CMAKE_SOURCE_DIR}/renderboi/config/project_env.config.hpp" "${CMAKE_SOURCE_DIR}/renderboi/project_env.hpp")



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
set(GLAD_LIB_NAME "glad")
set(GETOPT_WIN_LIB_NAME "getopt")
set(STB_IMAGE_LIB_NAME "stb_image")
set(GLM_LIB_NAME "glm")
set(GLFW3_LIB_NAME "glfw3")
set(RB_UTILITIES_LIB_NAME "renderboi.utilities")
set(RB_CORE_LIB_NAME "renderboi.core")
set(RB_WINDOW_LIB_NAME "renderboi.window")
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

set(RB_UTILITIES_SOURCE_FILES
    gl_utilities.cpp
    gl_utilities.hpp
    resource_locator.cpp
    resource_locator.hpp
    to_string.hpp
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
    window_factory.hpp
    window_backend.hpp
    glfw3/glfw3_adapter.cpp
    glfw3/glfw3_adapter.hpp
    glfw3/glfw3_window.cpp
    glfw3/glfw3_window.hpp
    glfw3/glfw3_window_callbacks.cpp
    glfw3/glfw3_window_callbacks.hpp
    glfw3/glfw3_window_factory.cpp
    glfw3/glfw3_window_factory.hpp
)

set(RB_TOOLBOX_SOURCE_FILES
    common_macros.hpp
    factory.cpp
    factory.hpp
    input_splitter.cpp
    input_splitter.hpp 
    script.cpp
    script.hpp 
    controls/control.cpp
    controls/control.hpp
    controls/control_event_translator.hpp
    controls/control_scheme_manager.hpp
    controls/controlled_entity_manager.hpp
    interfaces/action_event_receiver.hpp
    interfaces/control_binding_provider.hpp
    interfaces/default_control_scheme_provider.hpp
    interfaces/mesh_generator.hpp
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
    scene/components/light_component.cpp
    scene/components/light_component.hpp 
    scene/components/mesh_component.cpp
    scene/components/mesh_component.hpp 
    scene/components/script_component.cpp
    scene/components/script_component.hpp 
    runnables/basic_window_manager.cpp
    runnables/basic_window_manager.hpp 
    runnables/camera_aspect_ratio_manager.cpp
    runnables/camera_aspect_ratio_manager.hpp 
    runnables/input_logger.cpp
    runnables/input_logger.hpp
    runnables/keyboard_movement_script.cpp
    runnables/keyboard_movement_script.hpp 
    runnables/mouse_camera_manager.cpp
    runnables/mouse_camera_manager.hpp 
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
#                           Include subdirectories                            #
#                                                                             #
###############################################################################

# Import getopt target only for windows (linux has it)
if(WIN32)
    add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/getoptwin ${CMAKE_BINARY_DIR}/${GETOPT_LIB_NAME})
endif()

# Import GLAD targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glad ${CMAKE_BINARY_DIR}/${GLAD_LIB_NAME})

# Import stb_image targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/stb_image ${CMAKE_BINARY_DIR}/${STB_IMAGE_LIB_NAME})

# Import GLM targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glm ${CMAKE_BINARY_DIR}/${GLM_LIB_NAME})

# Import RenderBoi.Utilities targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/utilities ${CMAKE_BINARY_DIR}/${RB_UTILITIES_LIB_NAME})

# Import RenderBoi.Core targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/core ${CMAKE_BINARY_DIR}/${RB_CORE_LIB_NAME})

# Import RenderBoi.Window targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/window ${CMAKE_BINARY_DIR}/${RB_WINDOW_LIB_NAME})

# Import RenderBoi.Toolbox targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/toolbox ${CMAKE_BINARY_DIR}/${RB_TOOLBOX_LIB_NAME})

# Import RenderBoi.Examples targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/examples ${CMAKE_BINARY_DIR}/${RB_EXAMPLES_LIB_NAME})



###############################################################################
#                                                                             #
#                         Build RenderBoi executable                          #
#                                                                             #
###############################################################################

# Build executable
add_executable("RenderBoi"
    ${RENDERBOI_MODULE_LOCATION}/main.cpp
    ${RENDERBOI_MODULE_LOCATION}/main_functions.hpp
    ${RENDERBOI_MODULE_LOCATION}/main_functions.cpp
    ${RENDERBOI_MODULE_LOCATION}/project_env.hpp
    ${RENDERBOI_MODULE_LOCATION}/renderboi_parameters.hpp
)   

add_dependencies("RenderBoi"
    "export_${RB_UTILITIES_LIB_NAME}_lib"
    "export_${RB_UTILITIES_LIB_NAME}_headers"
    "export_${RB_WINDOW_LIB_NAME}_lib"
    "export_${RB_WINDOW_LIB_NAME}_headers"
    "export_${RB_EXAMPLES_LIB_NAME}_lib"
    "export_${RB_EXAMPLES_LIB_NAME}_headers"
)

if(WIN32)
    add_dependencies("RenderBoi"
        "export_${GETOPT_LIB_NAME}_lib"
        "export_${GETOPT_LIB_NAME}_headers"
    )
endif()

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
