
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