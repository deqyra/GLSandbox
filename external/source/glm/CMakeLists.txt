cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(GLM_LIB_NAME "glm")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${GLM_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include
        FILES_MATCHING
        PATTERN \"*.hpp\"
        PATTERN \"*.h\"
        PATTERN \"*.inl\"
   )\n"
)

add_custom_target("export_${GLM_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${GLM_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${GLM_LIB_NAME}_headers.cmake
)
