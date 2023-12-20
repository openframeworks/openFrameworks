define_addon(ofxAssimpModelLoader)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(ofxAssimpModelLoader assimp::assimp)