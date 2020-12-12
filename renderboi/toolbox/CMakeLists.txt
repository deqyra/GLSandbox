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

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_TOOLBOX_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build core lib
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/core ${CMAKE_BINARY_DIR}/renderboi.core)

# Build window lib
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/window ${CMAKE_BINARY_DIR}/renderboi.window)

# Build toolbox lib
add_library(${RB_TOOLBOX_LIB_NAME} ${STATIC_OR_SHARED}
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
add_dependencies(${RB_TOOLBOX_LIB_NAME}
    "export_${RB_CORE_LIB_NAME}_lib"
    "export_${RB_CORE_LIB_NAME}_headers"
    "export_${RB_WINDOW_LIB_NAME}_lib"
    "export_${RB_WINDOW_LIB_NAME}_headers"
)
target_include_directories(${RB_TOOLBOX_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_TOOLBOX_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_TOOLBOX_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${GLAD_LIB_NAME}
    ${RB_CORE_LIB_NAME}
    ${RB_WINDOW_LIB_NAME}
)

add_custom_target("export_${RB_TOOLBOX_LIB_NAME}_lib")
add_dependencies("export_${RB_TOOLBOX_LIB_NAME}_lib" ${RB_TOOLBOX_LIB_NAME})
add_custom_command(
  TARGET "export_${RB_TOOLBOX_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_TOOLBOX_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target("export_${RB_TOOLBOX_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${RB_TOOLBOX_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_TOOLBOX_LIB_NAME}_headers.cmake
)
