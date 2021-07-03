cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${GLAD_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include
        FILES_MATCHING
        PATTERN \"*.h\"
   )\n"
)

# Build glad lib
add_library(${GLAD_LIB_NAME} ${STATIC_OR_SHARED}
    gl.c
    gl.h
    KHR/khrplatform.h
)
set_target_properties(${GLAD_LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
add_compile_definitions(GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)

add_custom_target("export_${GLAD_LIB_NAME}_lib")
add_dependencies("export_${GLAD_LIB_NAME}_lib" ${GLAD_LIB_NAME})
add_custom_command(
  TARGET "export_${GLAD_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GLAD_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target("export_${GLAD_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${GLAD_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${GLAD_LIB_NAME}_headers.cmake
)
