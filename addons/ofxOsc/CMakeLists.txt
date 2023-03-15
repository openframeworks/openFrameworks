define_addon(ofxOsc)
target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src/osc)
target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src/ip)
target_include_directories(ofxOsc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/oscpack/src)