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

set( LIBNAME "renderboi.core")

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

# Build core lib
add_library( ${LIBNAME} ${STATIC_OR_SHARED}
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
target_include_directories( ${LIBNAME} PUBLIC ${INCLUDE_PATH} )
target_link_directories( ${LIBNAME} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( ${LIBNAME} PUBLIC stb_image glad ${CMAKE_DL_LIBS} )

add_custom_target( "export_${LIBNAME}_lib" )
add_dependencies( "export_${LIBNAME}_lib" ${LIBNAME})
add_custom_command(
  TARGET "export_${LIBNAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBNAME}> ${EXPORT_LOCATION}/lib/
)

add_custom_target( "export_${LIBNAME}_headers" )
add_custom_command(
  TARGET "export_${LIBNAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${LIBNAME}_headers.cmake
)
