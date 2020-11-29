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
add_library( "RenderBoi.Toolbox" ${STATIC_OR_SHARED}
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
target_include_directories( "RenderBoi.Toolbox" PUBLIC ${INCLUDE_PATH} )
target_link_directories( "RenderBoi.Toolbox" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( "RenderBoi.Toolbox" PUBLIC
    RenderBoi.Core
    RenderBoi.Window
    ${CMAKE_DL_LIBS}
)

# Install
install(
    TARGETS "RenderBoi.Toolbox"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/renderboi"
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.hxx"
)