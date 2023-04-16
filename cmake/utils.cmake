
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are a really bad idea. If you don't know what you're doing, please create a 'build' folder and call 'cmake ..' from within.")
endif()

# This function test-compiles a tiny program to see if a definition is defined or not (compiler internal)
function(TEST_IF_DEFINED OUTPUT_VAR DEFINE)

    set(SOURCE "
    #ifndef ${DEFINE}
    #error
    #endif
    int main() {}"
    )

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp ${SOURCE})

    try_compile(
        ${OUTPUT_VAR} 
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp
    )

    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/try_compile)

endfunction()

function(copy_file_after_build TARGET_NAME SOURCE_FILE TARGET_FILE)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SOURCE_FILE}" "${TARGET_FILE}")
endfunction()

# After compilation copy the dll files to the binary dir
function(of_copy_runtime_to_bin_dir_after_build TARGET BIN_DIR)
    if (NOT DEFINED OF_DEPS_SHARED_LIBS)
        message(FATAL_ERROR "INTERNAL ERROR: of_copy_runtime_to_bin_dir_after_build: OF_DEPS_SHARED_LIBS is not defined")
    endif()
    foreach(lib_path IN LISTS OF_DEPS_SHARED_LIBS)
        get_filename_component(lib_name ${lib_path} NAME)
        add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${lib_path}" "${BIN_DIR}/${lib_name}")
    endforeach()
endfunction()
