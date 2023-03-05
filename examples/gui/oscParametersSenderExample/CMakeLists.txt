
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(oscParametersSenderExample ${SOURCES})

target_link_libraries(oscParametersSenderExample ofxGui ofxOsc)
