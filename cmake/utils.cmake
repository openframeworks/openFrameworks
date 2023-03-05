
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
