
# Macro for defining an example with all of its properties
macro(define_example TARGET_NAME)       # A macro is executed in the caller's scope (current list is the caller)
    set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,debug,release>")
    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**)

    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
    target_link_libraries(${TARGET_NAME} of::openframeworks)

    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}")
    of_copy_runtime_to_bin_dir_after_build(${TARGET_NAME} "${BIN_DIR}")

    get_filename_component(parent_path ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
    get_filename_component(example_name ${parent_path} NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "openframeworks/examples/${example_name}")
endmacro()

# Function to let the build system copy resource files to the target location after compiling
function(copy_example_resource TARGET_NAME SOURCE_FILE) 
    get_filename_component(__FILENAME ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE} NAME)
    copy_file_after_build(${TARGET_NAME} 
        "${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,debug,release>/data/${__FILENAME}"
    )
endfunction()

# Folders with examples to build
add_subdirectory(3d)
# add_subdirectory(android)
# add_subdirectory(communication)
# add_subdirectory(computer_vision)
# add_subdirectory(events)
# add_subdirectory(gl)
# add_subdirectory(gles)
# add_subdirectory(graphics)
# add_subdirectory(gui)
# add_subdirectory(input_output)
# add_subdirectory(ios)
# add_subdirectory(math)
# add_subdirectory(shader)
# add_subdirectory(sound)
# add_subdirectory(strings)
# add_subdirectory(templates)
# add_subdirectory(threads)
# add_subdirectory(tvOS)
# add_subdirectory(video)
# add_subdirectory(windowing)
