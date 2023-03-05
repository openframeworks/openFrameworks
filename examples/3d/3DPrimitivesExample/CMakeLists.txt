
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(3DPrimitivesExample ${SOURCES})

copy_example_resource(3DPrimitivesExample bin/data/of.png)
