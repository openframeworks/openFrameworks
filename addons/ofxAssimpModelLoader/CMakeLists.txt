
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**.cpp)
define_addon(ofxAssimpModelLoader ${SOURCES})

target_link_libraries(ofxAssimpModelLoader of::assimp)
