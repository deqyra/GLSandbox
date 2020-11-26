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

# Build core lib
add_library( "RenderBoi.Core" ${STATIC_OR_SHARED}
    common_macros.hpp
    camera.cpp
    camera.hpp
    factory.cpp
    factory.hpp
    input_processor.cpp
    input_processor.hpp
    material.cpp
    material.hpp
    materials.hpp
    mesh.cpp
    mesh.hpp
    mesh_generator.hpp
    object_transform.cpp
    object_transform.hpp
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
    scene/input_processing_script.cpp
    scene/input_processing_script.hpp 
    scene/scene.cpp
    scene/scene.hpp 
    scene/scene_object_metadata.hpp 
    scene/scene_object.cpp
    scene/scene_object.hpp 
    scene/scene_renderer.cpp
    scene/scene_renderer.hpp 
    scene/script.cpp
    scene/script.hpp 
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
    ubo/ubo_info.hpp 
    ubo/uniform_buffer_object.cpp
    ubo/uniform_buffer_object.hpp 
    ubo/light_ubo.cpp
    ubo/light_ubo.hpp 
    ubo/matrix_ubo.cpp
    ubo/matrix_ubo.hpp 
)
target_include_directories( "RenderBoi.Core" PUBLIC ${INCLUDE_PATH} )
target_link_directories( "RenderBoi.Core" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( "RenderBoi.Core" PUBLIC cpptools stbi glad ${CMAKE_DL_LIBS} )

# Install
install(
    TARGETS "RenderBoi.Core"
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