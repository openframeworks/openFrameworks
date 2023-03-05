
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(quaternionLatLongExample ${SOURCES})

copy_example_resource(quaternionLatLongExample bin/data/earth.jpg)
