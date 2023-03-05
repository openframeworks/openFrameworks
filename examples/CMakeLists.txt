
macro(define_example TARGET_NAME)       # A macro is executed in the caller's scope (current list is the caller)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${ARGN})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

    target_compile_features(openframeworks PUBLIC cxx_std_17)
    set_target_properties(openframeworks PROPERTIES CXX_EXTENSIONS OFF)

    target_link_libraries(${TARGET_NAME} of::openframeworks)

    get_filename_component(DIR_PATH ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
    get_filename_component(DIR_NAME ${DIR_PATH} NAME)
    if (NOT OF_IS_MAIN_PROJECT) # If including OF from another project
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "openframeworks/examples/${DIR_NAME}")
    else ()                     # If building OF directly in the root
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples-${DIR_NAME}")
    endif ()

    set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,Debug,Release>")
endmacro()

function(copy_example_resource TARGET_NAME SOURCE_FILE) 
    get_filename_component(__FILENAME ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE} NAME)
    copy_file_after_build(
        ${TARGET_NAME} 
        "${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE}" 
        "${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,Debug,Release>/data/${__FILENAME}"
    )
endfunction()

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
