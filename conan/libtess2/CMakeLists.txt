cmake_minimum_required(VERSION 3.16)
project(libtess2)

set(LIBTESS2_SOURCES
    source/Source/bucketalloc.c
    source/Source/bucketalloc.h
    source/Source/dict.c
    source/Source/dict.h
    source/Source/geom.c
    source/Source/geom.h
    source/Source/mesh.c
    source/Source/mesh.h
    source/Source/priorityq.c
    source/Source/priorityq.h
    source/Source/sweep.c
    source/Source/sweep.h
    source/Source/tess.c
    source/Source/tess.h
)
add_library(libtess2 STATIC ${LIBTESS2_SOURCES})
add_library(libtess2::libtess2 ALIAS libtess2)
target_include_directories(libtess2 PUBLIC source/Source source/Include)
target_compile_features(libtess2 PUBLIC c_std_99)

set_target_properties(libtess2 PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/lib
)
