# Sets the minimum version of CMake required to build the native
# library.
cmake_minimum_required(VERSION  3.22.1)

project(openFrameworksAndroid LANGUAGES CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
if (NOT DEFINED OF_LIBRARY_TYPE)
    set(OF_LIBRARY_TYPE "SHARED")  # Default to SHARED if not set by Gradle # or "STATIC"
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Building openFrameworks as ${OF_LIBRARY_TYPE}")
set(LOCAL_PATH ${CMAKE_SOURCE_DIR})
set(PRJ_OF_ROOT ${LOCAL_PATH}/../../../../../)
set(PURE_OF_ROOT ${LOCAL_PATH}/../../../../../)
set(LIBS_ROOT ${PURE_OF_ROOT}/libs)
set(CORE_OF_ROOT ${PURE_OF_ROOT}/libs/openFrameworks)
set(PRJ_ADDONS_PATH ${PURE_OF_ROOT}/addons)
set(PRJ_SOURCE_PATH ${LIBS_ROOT}/openFrameworks)
set(OF_SOURCE_DIR ${PRJ_SOURCE_PATH})
set(PRJ_LIBS_ROOT ${PURE_OF_ROOT}/libs)
set(OF_ANDROID ${PURE_OF_ROOT}/libs/openFrameworksCompiled/project/android)
set(OF_ANDROID_OUTPUT ${PURE_OF_ROOT}/libs/openFrameworksCompiled/lib/android)
set(PRJ_OFX_ANDROID_PATH ${PRJ_ADDONS_PATH}/ofxAndroid)
set(PRJ_OFX_ANDROID_CPP_PATH ${PRJ_ADDONS_PATH}/ofxAndroid/src)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CONFIGURATION_BUILD_DIR ${OF_ANDROID_OUTPUT})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -fno-short-enums -fPIE -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fno-short-enums -fPIE -fPIC -fexceptions")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-export-dynamic")
file(GLOB OF_SOURCES
        ${OF_SOURCE_DIR}/*.cpp
        ${OF_SOURCE_DIR}/3d/*.cpp
        ${OF_SOURCE_DIR}/app/*.cpp
        ${OF_SOURCE_DIR}/communication/*.cpp
        ${OF_SOURCE_DIR}/events/*.cpp
        ${OF_SOURCE_DIR}/gl/*.cpp
        ${OF_SOURCE_DIR}/graphics/*.cpp
        ${OF_SOURCE_DIR}/math/*.cpp
        ${OF_SOURCE_DIR}/sound/*.cpp
        ${OF_SOURCE_DIR}/types/*.cpp
        ${OF_SOURCE_DIR}/utils/*.cpp
        ${OF_SOURCE_DIR}/video/*.cpp
        ${PRJ_OFX_ANDROID_CPP_PATH}/*.cpp
        ${PRJ_ADDONS_PATH}/ofxAddons/src/*.cpp
        ${PRJ_ADDONS_PATH}/ofxAccelerometer/src/ofxAccelerometer.cpp
)
# exclusions
list(REMOVE_ITEM OF_SOURCES
        "${OF_SOURCE_DIR}/app/ofAppEGLWindow.cpp"
        "${OF_SOURCE_DIR}/sound/ofMediaFoundationSoundPlayer.cpp"
        "${OF_SOURCE_DIR}/video/ofGstVideoPlayer.cpp"
        "${OF_SOURCE_DIR}/video/ofGstVideoGrabber.cpp"
        "${OF_SOURCE_DIR}/video/ofMediaFoundationPlayer.cpp"
        "${OF_SOURCE_DIR}/video/ofDirectShowPlayer.cpp"
        "${OF_SOURCE_DIR}/video/ofDirectShowGrabber.cpp"
)

add_library(openFrameworksAndroid ${OF_LIBRARY_TYPE} ${OF_SOURCES})

target_include_directories(openFrameworksAndroid PRIVATE
        ${OF_SOURCE_DIR}
        ${OF_SOURCE_DIR}/3d
        ${OF_SOURCE_DIR}/app
        ${OF_SOURCE_DIR}/communication
        ${OF_SOURCE_DIR}/events
        ${OF_SOURCE_DIR}/gl
        ${OF_SOURCE_DIR}/graphics
        ${OF_SOURCE_DIR}/math
        ${OF_SOURCE_DIR}/sound
        ${OF_SOURCE_DIR}/types
        ${OF_SOURCE_DIR}/utils
        ${OF_SOURCE_DIR}/video
)

set_target_properties(openFrameworksAndroid
		PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY
		"${CONFIGURATION_BUILD_DIR}/${ANDROID_ABI}")

include_directories(
		${PRJ_SOURCE_PATH}/3d
		${PRJ_SOURCE_PATH}/app
		${PRJ_SOURCE_PATH}/communication
		${PRJ_SOURCE_PATH}/events
		${PRJ_SOURCE_PATH}/gl
		${PRJ_SOURCE_PATH}/graphics
		${PRJ_SOURCE_PATH}/math
		${PRJ_SOURCE_PATH}/sound
		${PRJ_SOURCE_PATH}/types
		${PRJ_SOURCE_PATH}/utils
		${PRJ_SOURCE_PATH}/video
		${PRJ_SOURCE_PATH}
        ${PRJ_OFX_ANDROID_CPP_PATH}/
		${PURE_OF_ROOT}/addons/ofxAndroid/src
		${PURE_OF_ROOT}/addons/ofxAccelerometer/src
		${PRJ_LIBS_ROOT}/FreeImage/include
		${PRJ_LIBS_ROOT}/freetype/include
		${PRJ_LIBS_ROOT}/freetype/include/freetype2
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/config
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal/services
		${PRJ_LIBS_ROOT}/glm/include
        ${PRJ_LIBS_ROOT}/brotli/include
        ${PRJ_LIBS_ROOT}/json/include
        ${PRJ_LIBS_ROOT}/libpng/include
        ${PRJ_LIBS_ROOT}/fmt/include
		${PRJ_LIBS_ROOT}/pugixml/include
		${PRJ_LIBS_ROOT}/json/include
		${PRJ_LIBS_ROOT}/tess2/include
		${PRJ_LIBS_ROOT}/utf8/include
        ${PRJ_LIBS_ROOT}/tess2/include
        ${PRJ_LIBS_ROOT}/zlib/include
		${PRJ_LIBS_ROOT}/uriparser/include
        ${PRJ_LIBS_ROOT}/openssl/include
        ${PRJ_LIBS_ROOT}/curl/include
)

add_definitions(-DANDROID)

find_library(ANDROID_LIB NAMES android)
find_library(LOG_LIB NAMES log)
find_library(GLES1_LIB NAMES GLESv1_CM)
find_library(GLES2_LIB NAMES GLESv2)
find_library(GLES3_LIB NAMES GLESv3)

target_link_libraries(openFrameworksAndroid
		android
		log
		m
		z
		dl
		)

find_library(atomic_lib atomic)
if(atomic_lib)
  target_link_libraries(openFrameworksAndroid ${atomic_lib})
endif()

find_library(mediandk_lib mediandk)

target_link_libraries(openFrameworksAndroid
    ${mediandk_lib}
)

target_link_libraries(openFrameworksAndroid
        ${ANDROID_LIB}
        ${GLES2_LIB}
        ${GLES3_LIB}
        ${GLES1_LIB}
        ${LOG_LIB}
)

target_link_libraries(openFrameworksAndroid
        ${PRJ_LIBS_ROOT}/brotli/lib/android/${ANDROID_ABI}/libbrotlicommon.a
        ${PRJ_LIBS_ROOT}/brotli/lib/android/${ANDROID_ABI}/libbrotlidec.a
        ${PRJ_LIBS_ROOT}/brotli/lib/android/${ANDROID_ABI}/libbrotlienc.a
        ${PRJ_LIBS_ROOT}/fmt/lib/android/${ANDROID_ABI}/libfmt.a
        ${PRJ_LIBS_ROOT}/zlib/lib/android/${ANDROID_ABI}/zlib.a
        ${PRJ_LIBS_ROOT}/libpng/lib/android/${ANDROID_ABI}/libpng.a
        ${PRJ_LIBS_ROOT}/FreeImage/lib/android/${ANDROID_ABI}/libFreeImage.a
        ${PRJ_LIBS_ROOT}/freetype/lib/android/${ANDROID_ABI}/libfreetype.a
        ${PRJ_LIBS_ROOT}/pugixml/lib/android/${ANDROID_ABI}/libpugixml.a
        ${PRJ_LIBS_ROOT}/tess2/lib/android/${ANDROID_ABI}/libtess2.a
        ${PRJ_LIBS_ROOT}/uriparser/lib/android/${ANDROID_ABI}/liburiparser.a
        ${PRJ_LIBS_ROOT}/openssl/lib/android/${ANDROID_ABI}/libssl.a
        ${PRJ_LIBS_ROOT}/openssl/lib/android/${ANDROID_ABI}/libcrypto.a
        ${PRJ_LIBS_ROOT}/curl/lib/android/${ANDROID_ABI}/libcurl.a
)
