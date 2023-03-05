
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(3DModelLoaderExample ${SOURCES})

target_link_libraries(3DModelLoaderExample of::ofxAssimpModelLoader)

# copy_example_resource(3DModelLoaderExample bin/data/penguin.jpg)
