# Set up external dependencies
include(ExternalProject)
ExternalProject_Add(${GLFW3_LIB_NAME}_source
    GIT_REPOSITORY https://github.com/glfw/glfw
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXPORT_LOCATION}
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
)

# Detect EGL platform
set(EGL_DETECTED 0)
if(UNIX)
    if (DEFINED ENV{WAYLAND_DISPLAY})
        set(EGL_DETECTED 1)
        message(STATUS "Wayland detected, EGL compatibility enabled.")
    endif()
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/env_info.hpp")

# Generate script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
   )\n"
)

# Build window lib
add_library(${RB_WINDOW_LIB_NAME} ${STATIC_OR_SHARED}
    ${RB_WINDOW_SOURCE_FILES}
)
add_dependencies(${RB_WINDOW_LIB_NAME}
    "${GLFW3_LIB_NAME}_source"
    "export_${GLAD_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_headers"
)
target_include_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_WINDOW_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_WINDOW_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${GLFW3_LIB_NAME}
    ${GLAD_LIB_NAME}
    pthread
)

# Custom target to export window lib
add_custom_target("export_${RB_WINDOW_LIB_NAME}_lib")
add_dependencies("export_${RB_WINDOW_LIB_NAME}_lib" ${RB_WINDOW_LIB_NAME})
add_custom_command(
  TARGET "export_${RB_WINDOW_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_WINDOW_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

# Custom target to export window headers
add_custom_target("export_${RB_WINDOW_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${RB_WINDOW_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_WINDOW_LIB_NAME}_headers.cmake
)
