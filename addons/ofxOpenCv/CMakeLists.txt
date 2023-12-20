define_addon(ofxOpenCv)

find_package(OpenCV REQUIRED)
target_link_libraries(ofxOpenCv opencv::opencv)

# if (WIN32)      # We only need ippicv on Windows
#     target_link_libraries(ofxOpenCv opencv::ippicv)
# endif()
