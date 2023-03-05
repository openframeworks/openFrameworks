
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(pointPickerExample ${SOURCES})

copy_example_resource(pointPickerExample bin/data/lofi-bunny.ply)
