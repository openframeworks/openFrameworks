
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**.cpp)
define_addon(ofxGui ${SOURCES})
