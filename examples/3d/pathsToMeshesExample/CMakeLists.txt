
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(pathsToMeshesExample ${SOURCES})

copy_example_resource(pathsToMeshesExample bin/data/frabk.ttf)
