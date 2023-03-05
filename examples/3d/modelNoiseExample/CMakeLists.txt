
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(modelNoiseExample ${SOURCES})

target_link_libraries(modelNoiseExample of::ofxAssimpModelLoader)
