define_addon(ofxKinect)

target_include_directories(ofxKinect PUBLIC libs/libfreenect/include)
target_include_directories(ofxKinect PUBLIC libs/libfreenect/src)
target_include_directories(ofxKinect PUBLIC src/extra)

if (WIN32)
    target_include_directories(ofxKinect PUBLIC libs/libfreenect/platform/windows)
endif ()

find_package(libusb REQUIRED)
target_link_libraries(ofxKinect libusb::libusb)