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

set(RB_CORE_LIB_NAME "renderboi.core")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_CORE_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build GLAD
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glad ${CMAKE_BINARY_DIR}/glad)
set(GLAD_LIB_NAME glad)

# Build stb_image
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/stb_image ${CMAKE_BINARY_DIR}/stb_image)
set(STB_IMAGE_LIB_NAME stb_image)

# Export GLM
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glm ${CMAKE_BINARY_DIR}/glm)
set(GLM_LIB_NAME glm)

# Build core lib
add_library(${RB_CORE_LIB_NAME} ${STATIC_OR_SHARED}
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
add_dependencies(${RB_CORE_LIB_NAME}
    "cpptools_source"
    "export_${GLAD_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_headers"
    "export_${STB_IMAGE_LIB_NAME}_lib"
    "export_${STB_IMAGE_LIB_NAME}_headers"
    "export_${GLM_LIB_NAME}_headers"
)
target_include_directories(${RB_CORE_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_CORE_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_CORE_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${STB_IMAGE_LIB_NAME}
    ${GLAD_LIB_NAME}
)

add_custom_target("export_${RB_CORE_LIB_NAME}_lib")
add_dependencies("export_${RB_CORE_LIB_NAME}_lib" ${RB_CORE_LIB_NAME})
add_custom_command(
  TARGET "export_${RB_CORE_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_CORE_LIB_NAME}> ${EXPORT_LOCATION}/lib/
)

add_custom_target("export_${RB_CORE_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${RB_CORE_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_CORE_LIB_NAME}_headers.cmake
)
