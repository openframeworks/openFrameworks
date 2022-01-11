# Sets the minimum version of CMake required to build the native
# library.
cmake_minimum_required(VERSION  3.10.2)

project(openFrameworksAndroid LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE on)

set(LOCAL_PATH ${CMAKE_SOURCE_DIR})
set(PRJ_OF_ROOT ${LOCAL_PATH}/../../../../../)
set(PURE_OF_ROOT ${LOCAL_PATH}/../../../../../)
set(LIBS_ROOT ${PURE_OF_ROOT}/libs)
set(CORE_OF_ROOT ${PURE_OF_ROOT}/libs/openFrameworks)

set(PRJ_ADDONS_PATH ${PURE_OF_ROOT}/addons)
set(PRJ_SOURCE_PATH ${LIBS_ROOT}/openFrameworks)
set(PRJ_LIBS_ROOT ${PURE_OF_ROOT}/libs)

set(OF_ANDROID ${PURE_OF_ROOT}/libs/openFrameworksCompiled/project/android)
set(OF_ANDROID_OUTPUT ${PURE_OF_ROOT}/libs/openFrameworksCompiled/lib/android)

set(PRJ_OFX_ANDROID_PATH ${PRJ_ADDONS_PATH}/ofxAndroid)
set(PRJ_OFX_ANDROID_CPP_PATH ${PRJ_OFX_ANDROID_PATH}/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


set(distribution_DIR ${OF_ANDROID_OUTPUT})


#if(MSVC)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
#else(MSVC)
#	target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#endif(MSVC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -error-limit=0 -std=c17 -Oz -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections ")
set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS} -error-limit=0 -std=c++17 -Oz -stdlib=libc++ -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-export-dynamic")
# Creates the project's shared lib: libnative-lib.so.
# The lib is loaded by this project's Java code in MainActivity.java:
#     System.loadLibrary("native-lib");
# The lib name in both places must match.
# Specify your native library name, type and the cpp source files.
add_library( openFrameworksAndroid
		SHARED
		${PRJ_SOURCE_PATH}/utils/ofFileUtils.cpp
		${PRJ_SOURCE_PATH}/utils/ofFpsCounter.cpp
		${PRJ_SOURCE_PATH}/utils/ofLog.cpp
		${PRJ_SOURCE_PATH}/utils/ofMatrixStack.cpp
		${PRJ_SOURCE_PATH}/utils/ofSystemUtils.cpp
		${PRJ_SOURCE_PATH}/utils/ofThread.cpp
		${PRJ_SOURCE_PATH}/utils/ofTimer.cpp
		${PRJ_SOURCE_PATH}/utils/ofURLFileLoader.cpp
		${PRJ_SOURCE_PATH}/utils/ofUtils.cpp
		${PRJ_SOURCE_PATH}/utils/ofXml.cpp
		${PRJ_SOURCE_PATH}/sound/ofSoundBaseTypes.cpp
		${PRJ_SOURCE_PATH}/sound/ofSoundBuffer.cpp
		${PRJ_SOURCE_PATH}/sound/ofSoundPlayer.cpp
		${PRJ_SOURCE_PATH}/sound/ofSoundStream.cpp
		${PRJ_SOURCE_PATH}/sound/ofOpenALSoundPlayer.cpp
		${PRJ_SOURCE_PATH}/3d/of3dPrimitives.cpp
		${PRJ_SOURCE_PATH}/3d/of3dUtils.cpp
		${PRJ_SOURCE_PATH}/3d/ofCamera.cpp
		${PRJ_SOURCE_PATH}/3d/ofEasyCam.cpp
		${PRJ_SOURCE_PATH}/3d/ofNode.cpp
		${PRJ_SOURCE_PATH}/app/ofMainLoop.cpp
		${PRJ_SOURCE_PATH}/app/ofAppRunner.cpp
		${PRJ_SOURCE_PATH}/app/ofBaseApp.cpp
		${PRJ_SOURCE_PATH}/communication/ofSerial.cpp
		${PRJ_SOURCE_PATH}/events/ofEvents.cpp
		${PRJ_SOURCE_PATH}/gl/ofBufferObject.cpp
		${PRJ_SOURCE_PATH}/gl/ofFbo.cpp
		${PRJ_SOURCE_PATH}/gl/ofGLProgrammableRenderer.cpp
		${PRJ_SOURCE_PATH}/gl/ofGLRenderer.cpp
		${PRJ_SOURCE_PATH}/gl/ofGLUtils.cpp
		${PRJ_SOURCE_PATH}/gl/ofLight.cpp
		${PRJ_SOURCE_PATH}/gl/ofMaterial.cpp
		${PRJ_SOURCE_PATH}/gl/ofShader.cpp
		${PRJ_SOURCE_PATH}/gl/ofTexture.cpp
		${PRJ_SOURCE_PATH}/gl/ofVbo.cpp
		${PRJ_SOURCE_PATH}/gl/ofVboMesh.cpp
		${PRJ_SOURCE_PATH}/graphics/of3dGraphics.cpp
		${PRJ_SOURCE_PATH}/graphics/ofBitmapFont.cpp
		${PRJ_SOURCE_PATH}/graphics/ofGraphics.cpp
		${PRJ_SOURCE_PATH}/graphics/ofGraphicsBaseTypes.cpp
		${PRJ_SOURCE_PATH}/graphics/ofImage.cpp
		${PRJ_SOURCE_PATH}/graphics/ofPath.cpp
		${PRJ_SOURCE_PATH}/graphics/ofPixels.cpp
		${PRJ_SOURCE_PATH}/graphics/ofRendererCollection.cpp
		${PRJ_SOURCE_PATH}/graphics/ofTessellator.cpp
		${PRJ_SOURCE_PATH}/graphics/ofTrueTypeFont.cpp
		${PRJ_SOURCE_PATH}/math/ofMath.cpp
		${PRJ_SOURCE_PATH}/math/ofMatrix3x3.cpp
		${PRJ_SOURCE_PATH}/math/ofMatrix4x4.cpp
		${PRJ_SOURCE_PATH}/math/ofQuaternion.cpp
		${PRJ_SOURCE_PATH}/math/ofVec2f.cpp
		${PRJ_SOURCE_PATH}/math/ofVec4f.cpp
		${PRJ_SOURCE_PATH}/types/ofColor.cpp
		${PRJ_SOURCE_PATH}/types/ofParameter.cpp
		${PRJ_SOURCE_PATH}/types/ofParameterGroup.cpp
		${PRJ_SOURCE_PATH}/types/ofRectangle.cpp
		${PRJ_SOURCE_PATH}/video/ofVideoBaseTypes.cpp
		${PRJ_SOURCE_PATH}/video/ofVideoGrabber.cpp
		${PRJ_SOURCE_PATH}/video/ofVideoPlayer.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofAppAndroidWindow.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidAccelerometer.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidLogChannel.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidSoundPlayer.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidSoundStream.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidUtils.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidVibrator.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidVideoGrabber.cpp
		${PRJ_OFX_ANDROID_CPP_PATH}/ofxAndroidVideoPlayer.cpp
		${PRJ_ADDONS_PATH}/ofxAccelerometer/src/ofxAccelerometer.cpp
		${PRJ_ADDONS_PATH}/ofxXmlSettings/src/ofxXmlSettings.cpp
		${PRJ_ADDONS_PATH}/ofxXmlSettings/libs/tinyxml.cpp
		${PRJ_ADDONS_PATH}/ofxXmlSettings/libs/tinyxmlerror.cpp
		${PRJ_ADDONS_PATH}/ofxXmlSettings/libs/tinyxmlparser.cpp )

set_target_properties(openFrameworksAndroid
		PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY
		"${distribution_DIR}/${ANDROID_ABI}")

# Specifies a path to native header files.
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
		${PURE_OF_ROOT}/addons/ofxAndroid/src
		${PURE_OF_ROOT}/addons/ofxAccelerometer/src
		${PURE_OF_ROOT}/addons/ofxXmlSettings/src
		${PURE_OF_ROOT}/addons/ofxXmlSettings/libs
		${PRJ_LIBS_ROOT}/FreeImage/include
		${PRJ_LIBS_ROOT}/freetype/include
		${PRJ_LIBS_ROOT}/freetype/include/freetype2
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/config
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal
		${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal/services
		${PRJ_LIBS_ROOT}/glm/include
		${PRJ_LIBS_ROOT}/pugixml/include
		${PRJ_LIBS_ROOT}/json/include
		${PRJ_LIBS_ROOT}/tess2/include
		${PRJ_LIBS_ROOT}/utf8/include
		${PRJ_LIBS_ROOT}/uriparser/include
)

find_library(android-lib android)
find_library(log-lib log)
find_library(GLES1-lib GLESv1_CM)
find_library(GLES2-lib GLESv2)
find_library(GLES3-lib GLESv3)

#   [18, 24)         ES2 & ES3
#   [24, infinite)   ES2 & ES3 & Vulkan

if (${ANDROID_PLATFORM_LEVEL} LESS 24)
	set(OPENGL_LIB GLESv2)
else ()
	set(OPENGL_LIB GLESv2)
endif (${ANDROID_PLATFORM_LEVEL} LESS 24)

target_link_libraries(openFrameworksAndroid
		android
		EGL
		GLESv2
		GLESv3
		log
		m
		z
		dl
		)

target_link_libraries( openFrameworksAndroid
		${android-lib} )
target_link_libraries( openFrameworksAndroid
		${GLES2-lib} )
target_link_libraries( openFrameworksAndroid
		${GLES3-lib} )
target_link_libraries( openFrameworksAndroid
		${GLES1-lib} )
target_link_libraries( openFrameworksAndroid
		${log-lib} )

# Set the third-party library header files directory.
#include_directories(src/main/cpp/Libraries/include/)
# Link the thrid-party library with your native library.
target_link_libraries( openFrameworksAndroid #FreeImage
		${PRJ_LIBS_ROOT}/FreeImage/lib/android/${ANDROID_ABI}/libfreeimage.a )
target_link_libraries( openFrameworksAndroid #freetype
		${PRJ_LIBS_ROOT}/freetype/lib/android/${ANDROID_ABI}/libfreetype.a )
target_link_libraries( openFrameworksAndroid #pugixml
		${PRJ_LIBS_ROOT}/pugixml/lib/android/${ANDROID_ABI}/libpugixml.a)
target_link_libraries( openFrameworksAndroid #tess2
		${PRJ_LIBS_ROOT}/tess2/lib/android/${ANDROID_ABI}/libtess2.a)
target_link_libraries( openFrameworksAndroid #uriparser
		${PRJ_LIBS_ROOT}/uriparser/lib/android/${ANDROID_ABI}/liburiparser.a)

