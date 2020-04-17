# CMake configuration
    cmake_minimum_required( VERSION 3.1...3.15 )

    if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
        cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
    endif(  )

    set( CMAKE_CXX_COMPILER_WORKS 1 )   # Skips the compiler test, which is platform-dependent

# Toggles
    option( BUILD_STATIC_LIBS "Build libraries as static libraries" OFF )

    if( WIN32 )
        message( "Windows detected, forcing static libs." )
        set( BUILD_STATIC_LIBS ON )
    endif(  )

    if( ${BUILD_STATIC_LIBS} )
        set( STATIC_OR_SHARED "STATIC" )
    else(  )
        set( STATIC_OR_SHARED "SHARED" )
    endif(  )

# Project general info
    set( PROJECT_MAINTAINER_NAME "François Brachais" )
    set( PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com" )

# Project target structure
    set( PROJECT_TARGET_NAME "RenderBoi" )
    set( TOOLS_LIB_NAME "Tools" )
    set( GL_LOADER_LIB_NAME "GLAD" )
    set( STBI_LIB_NAME "STBImage" )
    set( CORE_LIB_NAME "${PROJECT_TARGET_NAME}.Core" )
    set( WINDOWING_LIB_NAME "${PROJECT_TARGET_NAME}.Windowing" )
    set( EXAMPLE_LIB_NAME "${PROJECT_TARGET_NAME}.Examples" )
    set( EXECUTABLE_NAME ${PROJECT_TARGET_NAME} )
    set( TEST_TARGET_NAME "tests" )
    set( COPY_ASSETS_TARGET_NAME "copy_assets" )
    set( COPY_DLLS_TARGET_NAME "copy_dlls" )

# Project library dependency names
    set( GLFW3_LIB "glfw3.lib" )
    set( ASSIMP_LIB "assimp.lib" )

# Copy assets folder
    add_custom_target( ${COPY_ASSETS_TARGET_NAME} )
    add_custom_command( TARGET ${COPY_ASSETS_TARGET_NAME}
                        COMMENT "Copying assets"
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ./assets )

# Copy lib folder
    add_custom_target( ${COPY_DLLS_TARGET_NAME} )
    add_custom_command( TARGET ${COPY_DLLS_TARGET_NAME}
                        COMMENT "Copying DLLs"
                        PRE_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/lib ./lib )

# Header configuration variables
    set( PROJECT_VERSION "1.0" )
    set( PROJECT_NAME "RenderBoi" )

    # Copyleft notice generation
        set( PROJECT_START_YEAR "2019" )
        string( TIMESTAMP CURRENT_YEAR "%Y" )
        # "(C) 2019"
        set( PROJECT_COPYLEFT "Copyleft ${PROJECT_START_YEAR}" )
        if( NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR )
            # "(C) 2019-2020"
            set( PROJECT_COPYLEFT "${PROJECT_COPYLEFT}-${CURRENT_YEAR}" )
        endif( )
        # "(C) 2019-2020 François Brachais <deqyra@gmail.com>"
        set( PROJECT_COPYLEFT "${PROJECT_COPYLEFT} ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>" )

# Header configuration
    configure_file( "project_macros.config.hpp" "${CMAKE_SOURCE_DIR}/project_macros.hpp" )

# Project definition
    project( ${PROJECT_TARGET_NAME} VERSION ${PROJECT_VERSION}
                                    DESCRIPTION "A rendering engine with a modest feature set"
                                    LANGUAGES C CXX )
    link_directories( ${CMAKE_SOURCE_DIR}/lib )
    include_directories( ${CMAKE_SOURCE_DIR}/include )

# C++ standard version
    # C++17 requires at least gcc 7 to be compiled
    set( CMAKE_CXX_STANDARD 17 )

# Build project
    # Build tools lib
        add_library( ${TOOLS_LIB_NAME} ${STATIC_OR_SHARED}
                    tools/string_tools.cpp                              tools/string_tools.hpp
                    tools/tree.hpp                                      tools/tree_node.hpp
                                                                        tools/make_basic_exception.hpp
                                                                        tools/enum_map.hpp
                    tools/exceptions/index_out_of_bounds_error.cpp      tools/exceptions/index_out_of_bounds_error.hpp
                    tools/exceptions/file_not_found_error.cpp           tools/exceptions/file_not_found_error.hpp 
                    tools/exceptions/range_bounds_exceeded_error.cpp    tools/exceptions/range_bounds_exceeded_error.hpp 
                    tools/exceptions/unmatched_array_size_error.cpp     tools/exceptions/unmatched_array_size_error.hpp
                    )

    # Build STBI lib
        add_library( ${STBI_LIB_NAME} ${STATIC_OR_SHARED}
                    stb_image.cpp                                       include/stb_image.hpp
                    )
    
    # Build GL loader
        add_library( ${GL_LOADER_LIB_NAME} ${STATIC_OR_SHARED} glad.c include/glad/glad.h )
        target_link_libraries( ${GL_LOADER_LIB_NAME} PUBLIC "${CMAKE_DL_LIBS}" )

    # Build windowing lib
        add_library( ${WINDOWING_LIB_NAME} ${STATIC_OR_SHARED}
                    windowing/glfw_window_callbacks.cpp                     windowing/glfw_window_callbacks.hpp
                    windowing/gl_window.cpp                                 windowing/gl_window.hpp
                    windowing/tools.cpp                                     windowing/tools.hpp
                    )
        target_link_libraries( ${WINDOWING_LIB_NAME} PUBLIC ${GLFW3_LIB} )
        target_link_libraries( ${WINDOWING_LIB_NAME} PUBLIC ${GL_LOADER_LIB_NAME} "${CMAKE_DL_LIBS}" )

    # Build core lib
        add_library( ${CORE_LIB_NAME} ${STATIC_OR_SHARED}
                                                                                core/gl_macros.hpp
                    core/camera.cpp                                             core/camera.hpp
                    core/input_processor.cpp                                    core/input_processor.hpp
                    core/material.cpp                                           core/material.hpp
                                                                                core/materials.hpp
                    core/mesh.cpp                                               core/mesh.hpp
                                                                                core/mesh_generator.hpp
                    core/positioned_object.cpp                                  core/positioned_object.hpp
                    core/shader.cpp                                             core/shader.hpp
                    core/texture_2d.cpp                                         core/texture_2d.hpp
                                                                                core/vertex.hpp
                    core/lights/directional_light.cpp                           core/lights/directional_light.hpp
                    core/lights/light.cpp                                       core/lights/light.hpp
                                                                                core/lights/light_type.hpp
                    core/lights/point_light.cpp                                 core/lights/point_light.hpp
                    core/lights/spot_light.cpp                                  core/lights/spot_light.hpp 
                    core/mesh_generators/axes_generator.cpp                     core/mesh_generators/axes_generator.hpp
                    core/mesh_generators/cube_generator.cpp                     core/mesh_generators/cube_generator.hpp
                    core/mesh_generators/tetrahedron_generator.cpp              core/mesh_generators/tetrahedron_generator.hpp
                    core/mesh_generators/torus_generator.cpp                    core/mesh_generators/torus_generator.hpp
                    core/scene/component.cpp                                    core/scene/component.hpp 
                                                                                core/scene/component_type.hpp 
                    core/scene/input_processing_script.cpp                      core/scene/input_processing_script.hpp 
                    core/scene/scene.cpp                                        core/scene/scene.hpp 
                    core/scene/scene_object.cpp                                 core/scene/scene_object.hpp 
                    core/scene/scene_renderer.cpp                               core/scene/scene_renderer.hpp 
                    core/scene/script.cpp                                       core/scene/script.hpp 
                                                                                core/scene/components/all_components.hpp 
                    core/scene/components/camera_component.cpp                  core/scene/components/camera_component.hpp 
                    core/scene/components/input_processing_script_component.cpp core/scene/components/input_processing_script_component.hpp 
                    core/scene/components/light_component.cpp                   core/scene/components/light_component.hpp 
                    core/scene/components/mesh_component.cpp                    core/scene/components/mesh_component.hpp 
                    core/scene/components/script_component.cpp                  core/scene/components/script_component.hpp 
                    core/scripts/basic_input_manager.cpp                        core/scripts/basic_input_manager.hpp 
                    core/scripts/fps_camera_script.cpp                          core/scripts/fps_camera_script.hpp 
                    core/scripts/lambda_script.cpp                              core/scripts/lambda_script.hpp 
                                                                                core/ubo/ubo_info.hpp 
                    core/ubo/uniform_buffer_object.cpp                          core/ubo/uniform_buffer_object.hpp 
                    core/ubo/light_ubo.cpp                                      core/ubo/light_ubo.hpp 
                    core/ubo/matrix_ubo.cpp                                     core/ubo/matrix_ubo.hpp 
                    )
        target_link_libraries( ${CORE_LIB_NAME} PUBLIC ${TOOLS_LIB_NAME} ${STBI_LIB_NAME} )
        target_link_libraries( ${CORE_LIB_NAME} PUBLIC ${GL_LOADER_LIB_NAME} "${CMAKE_DL_LIBS}" )

    # Build example lib
        add_library( ${EXAMPLE_LIB_NAME} ${STATIC_OR_SHARED}
                                                            examples/gl_sandbox.hpp
                    examples/lighting_sandbox.cpp           examples/lighting_sandbox.hpp
                    )
        target_link_libraries( ${EXAMPLE_LIB_NAME} PUBLIC ${CORE_LIB_NAME} ${TOOLS_LIB_NAME} ${WINDOWING_LIB_NAME} )

    # Build executable
        add_executable( ${EXECUTABLE_NAME}
                        main.cpp
                        project_macros.config.hpp           project_macros.hpp )        
        target_link_libraries( ${EXECUTABLE_NAME} PUBLIC
                                ${TOOLS_LIB_NAME}
                                ${CORE_LIB_NAME}
                                ${EXAMPLE_LIB_NAME}
                            )
        add_dependencies( ${EXECUTABLE_NAME} ${COPY_ASSETS_TARGET_NAME} )

# Build and run tests
    # enable_testing( )
    # add_executable( ${TEST_TARGET_NAME} )
    # target_link_libraries( ${TEST_TARGET_NAME} PUBLIC ${CORE_LIB_NAME} ${TOOLS_LIB_NAME} )
    # add_dependencies( ${TEST_TARGET_NAME} ${COPY_ASSETS_TARGET_NAME} ${COPY_DLLS_TARGET_NAME} )

    # add_test( NAME "tests" COMMAND ${TEST_TARGET_NAME} )
    # add_custom_command( TARGET ${TEST_TARGET_NAME}
    #                    COMMENT "Running tests"
    #                    POST_BUILD
    #                    COMMAND "./tests" )
