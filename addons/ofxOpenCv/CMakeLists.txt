
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**.cpp)
define_addon(ofxOpenCv ${SOURCES})

target_link_libraries(ofxOpenCv of::opencv of::ippicv)
