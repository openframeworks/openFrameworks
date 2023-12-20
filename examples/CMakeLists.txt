message(STATUS "[openframeworks] Configuring examples")

add_custom_target(build_all_examples)
set_target_properties(build_all_examples PROPERTIES FOLDER "openframeworks/examples")

# Macro for defining an example with all of its properties
macro(define_example TARGET_NAME)       # A macro is executed in the caller's scope (current list is the caller)
    set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,debug,release>")

    if (MSVC)     # In MSVC we add all files so the resource files are shown in the IDE
        file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**)
    else()        # But GCC doesn't like compiling resource files
        file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
    endif()

    add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
    target_link_libraries(${TARGET_NAME} of::openframeworks)

    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}")
    # of_copy_runtime_to_bin_dir_after_build(${TARGET_NAME} "${BIN_DIR}")

    get_filename_component(parent_path ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
    get_filename_component(example_name ${parent_path} NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "openframeworks/examples/${example_name}")

    # And copy the resource files to destination after building
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/bin/data")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E 
                copy_directory "${CMAKE_CURRENT_LIST_DIR}/bin/data" "${BIN_DIR}/data")
    endif()

    add_dependencies(build_all_examples ${TARGET_NAME})
endmacro()


# Folders with examples to build
add_subdirectory(3d)

if (OF_TARGET_ARCHITECTURE MATCHES "android")
    # add_subdirectory(android)
    message(WARNING "[openframeworks] Android Examples are not yet implemented. Feel free to implement them yourself, test and submit a PR!")
endif()

add_subdirectory(communication)
add_subdirectory(computer_vision)
add_subdirectory(events)
add_subdirectory(gl)

if (NOT OF_TARGET_ARCHITECTURE MATCHES "msvc" AND NOT OF_TARGET_ARCHITECTURE MATCHES "macos" AND NOT OF_TARGET_ARCHITECTURE MATCHES "linux")
    add_subdirectory(gles)
endif()

add_subdirectory(graphics)
add_subdirectory(gui)
add_subdirectory(input_output)

if (OF_TARGET_ARCHITECTURE MATCHES "ios")
    #add_subdirectory(ios)
    message(WARNING "[openframeworks] iOS Examples are not yet implemented. Feel free to implement them yourself, test and submit a PR!")
endif()

add_subdirectory(math)
add_subdirectory(shader)
add_subdirectory(sound)
add_subdirectory(strings)
add_subdirectory(templates)
add_subdirectory(threads)

if (OF_TARGET_ARCHITECTURE MATCHES "tvos")
    #add_subdirectory(tvOS)
    message(WARNING "[openframeworks] tvOS Examples are not yet implemented. Feel free to implement them yourself, test and submit a PR!")
endif()

add_subdirectory(video)
add_subdirectory(windowing)
