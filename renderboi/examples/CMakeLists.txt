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

# Build example lib
add_library( "RenderBoi.Examples" ${STATIC_OR_SHARED}
    gl_sandbox.hpp
    lighting_sandbox.cpp
    lighting_sandbox.hpp
    shadow_sandbox.cpp
    shadow_sandbox.hpp
)
target_include_directories( "RenderBoi.Examples" PUBLIC ${INCLUDE_PATH} )
target_link_directories( "RenderBoi.Examples" PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib )
target_link_libraries( "RenderBoi.Examples" PUBLIC "RenderBoi.Core" "cpptools" "RenderBoi.Window" )

# Install
install(
    TARGETS "RenderBoi.Examples"
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