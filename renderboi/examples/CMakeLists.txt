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

set( LIBNAME "renderboi.examples")

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

# Build example lib
add_library( ${LIBNAME} ${STATIC_OR_SHARED}
    gl_sandbox.hpp
    lighting_sandbox.cpp
    lighting_sandbox.hpp
    shadow_sandbox.cpp
    shadow_sandbox.hpp
)
target_include_directories( ${LIBNAME} PUBLIC ${INCLUDE_PATH} )
target_link_directories( ${LIBNAME} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( ${LIBNAME} PUBLIC
    cpptools
    glad
    renderboi.core
    renderboi.window
)

add_custom_target( "export_${LIBNAME}_lib" )
add_dependencies( "export_${LIBNAME}_lib" ${LIBNAME})
add_custom_command(
  TARGET "export_${LIBNAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBNAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target( "export_${LIBNAME}_headers" )
add_custom_command(
  TARGET "export_${LIBNAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${LIBNAME}_headers.cmake
)
