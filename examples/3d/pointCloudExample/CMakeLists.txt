
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(pointCloudExample ${SOURCES})

copy_example_resource(pointCloudExample bin/data/linzer.png)
