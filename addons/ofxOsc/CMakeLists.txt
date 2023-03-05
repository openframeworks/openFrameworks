
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**.cpp)
define_addon(ofxOsc ${SOURCES})

target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src/osc)
target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src/ip)
target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src)
