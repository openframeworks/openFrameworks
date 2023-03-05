
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(cameraLensOffsetExample ${SOURCES})

target_link_libraries(cameraLensOffsetExample of::ofxOpenCv)

copy_example_resource(cameraLensOffsetExample bin/data/haarcascade_frontalface_default.xml)
