cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(STB_IMAGE_LIB_NAME "stb_image")

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${STB_IMAGE_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build stb_image lib
add_library(${STB_IMAGE_LIB_NAME} ${STATIC_OR_SHARED}
    stb_image.cpp
    stb_image.hpp
)

add_custom_target("export_${STB_IMAGE_LIB_NAME}_lib")
add_dependencies("export_${STB_IMAGE_LIB_NAME}_lib" ${STB_IMAGE_LIB_NAME})
add_custom_command(
  TARGET "export_${STB_IMAGE_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${STB_IMAGE_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target("export_${STB_IMAGE_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${STB_IMAGE_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${STB_IMAGE_LIB_NAME}_headers.cmake
)
