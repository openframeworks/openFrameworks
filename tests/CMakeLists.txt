message(STATUS "[openframeworks] Configuring tests")

# Macro for defining a test with all of its properties
function(define_test TARGET_NAME)                          # A CMake function like this is executed in the caller's scope
    set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,debug,release>")

    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)

    add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
    target_link_libraries(${TARGET_NAME} of::openframeworks of::ofxUnitTests)

    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}")
    of_copy_runtime_to_bin_dir_after_build(${TARGET_NAME} "${BIN_DIR}")

    get_filename_component(parent_path ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
    get_filename_component(test_name ${parent_path} NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "openframeworks/tests/${test_name}")

    # And copy the resource files to destination after building
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/bin/data")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E 
                copy_directory "${CMAKE_CURRENT_LIST_DIR}/bin/data" "${BIN_DIR}/data")
    endif()

    add_dependencies(build_all_tests ${TARGET_NAME})

    # Add this test to the custom target that runs all tests
    add_custom_target(run_${TARGET_NAME} DEPENDS ${TARGET_NAME})
    add_custom_command(TARGET run_${TARGET_NAME} POST_BUILD COMMAND ${BIN_DIR}/${TARGET_NAME} WORKING_DIRECTORY ${BIN_DIR} COMMENT "Running test '${TARGET_NAME}'")
    add_dependencies(run_all_tests run_${TARGET_NAME})
    set_target_properties(run_${TARGET_NAME} PROPERTIES FOLDER "openframeworks/tests/${test_name}")

endfunction()

add_custom_target(build_all_tests)
set_target_properties(build_all_tests PROPERTIES FOLDER "openframeworks/tests")

add_custom_target(run_all_tests)
set_target_properties(run_all_tests PROPERTIES FOLDER "openframeworks/tests")

# Folders with tests to build
add_subdirectory(addons)
add_subdirectory(events)
add_subdirectory(graphics)
add_subdirectory(io)
add_subdirectory(math)
add_subdirectory(types)
add_subdirectory(utils)
