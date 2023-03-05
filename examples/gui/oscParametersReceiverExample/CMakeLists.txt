
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(oscParametersReceiverExample ${SOURCES})

target_link_libraries(oscParametersReceiverExample ofxGui ofxOsc)
