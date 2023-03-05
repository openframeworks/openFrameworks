
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(ofNodeExample ${SOURCES})

target_link_libraries(ofNodeExample of::ofxGui)

copy_example_resource(ofNodeExample bin/data/engine.wav)
copy_example_resource(ofNodeExample bin/data/horn.mp3)
copy_example_resource(ofNodeExample bin/data/reverse.mp3)
copy_example_resource(ofNodeExample bin/data/oflogo.ply)
copy_example_resource(ofNodeExample bin/data/truck.ply)
copy_example_resource(ofNodeExample bin/data/wheel.ply)
