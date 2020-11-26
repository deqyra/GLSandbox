cmake_minimum_required( VERSION 3.1...3.15 )

if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
endif(  )

# Build GL loader
add_library( "glad" ${STATIC_OR_SHARED}
    gl.c
    gl.h
    KHR/khrplatform.h
)
target_link_libraries( glad PUBLIC "${CMAKE_DL_LIBS}" )

# Install
install(
    TARGETS "glad"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.hxx"
)