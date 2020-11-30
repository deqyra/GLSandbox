cmake_minimum_required( VERSION 3.1...3.15 )

if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
    cmake_policy( VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
endif(  )

set( LIBNAME "stb_image")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${LIBNAME}_headers.cmake
    "file( COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include
        FILES_MATCHING
        PATTERN \"*.h\"
        PATTERN \"*.hpp\"
        PATTERN \"*.hxx\"
    )\n"
)

# Build stb_image lib
add_library( ${LIBNAME} ${STATIC_OR_SHARED}
    stb_image.cpp
    stb_image.hpp
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
