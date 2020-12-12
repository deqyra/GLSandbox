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

# Create script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_EXAMPLES_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.h\"
        PATTERN \"*.hpp\"
        PATTERN \"*.hxx\"
   )\n"
)

# Build toolbox lib
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/toolbox ${CMAKE_BINARY_DIR}/renderboi.toolbox)

# Build example lib
add_library(${RB_EXAMPLES_LIB_NAME} ${STATIC_OR_SHARED}
    gl_sandbox.hpp
    lighting_sandbox.cpp
    lighting_sandbox.hpp
    shadow_sandbox.cpp
    shadow_sandbox.hpp
)
add_dependencies(${RB_EXAMPLES_LIB_NAME}
    "export_${RB_TOOLBOX_LIB_NAME}_lib"
    "export_${RB_TOOLBOX_LIB_NAME}_headers"
)
target_include_directories(${RB_EXAMPLES_LIB_NAME} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include)
target_link_directories(${RB_EXAMPLES_LIB_NAME} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/lib)
target_link_libraries(${RB_EXAMPLES_LIB_NAME} PUBLIC
    ${CPPTOOLS_LIB_NAME}
    ${GLAD_LIB_NAME}
    ${RB_CORE_LIB_NAME}
    ${RB_WINDOW_LIB_NAME}
    ${RB_TOOLBOX_LIB_NAME}
)

add_custom_target("export_${RB_EXAMPLES_LIB_NAME}_lib")
add_dependencies("export_${RB_EXAMPLES_LIB_NAME}_lib" ${RB_EXAMPLES_LIB_NAME})
add_custom_command(
  TARGET "export_${RB_EXAMPLES_LIB_NAME}_lib"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_EXAMPLES_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

add_custom_target("export_${RB_EXAMPLES_LIB_NAME}_headers")
add_custom_command(
  TARGET "export_${RB_EXAMPLES_LIB_NAME}_headers"
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_EXAMPLES_LIB_NAME}_headers.cmake
)
